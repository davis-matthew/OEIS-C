/*
 * A084646  Hypotenuses for which there exist exactly 2 distinct integer
 *          triangles.
 * https://oeis.org/A084646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A084646 = { k * p^2 | p prime ≡ 1 (mod 4), k has no prime
 *            divisor ≡ 1 (mod 4) } - the sequence of hypotenuses that occur
 *            in exactly two distinct integer right triangles
 * Range:     u8 n <= 9, u16 n <= 1510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper macros ----------------------------------------------------- */
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

/* node stored in the min‑heap ------------------------------------- */
typedef struct {
    uint64_t val;   /* product k * p^2   */
    size_t   pk;    /* index in prime1[] */
    size_t   ki;    /* index in ks[]    */
} Node;

/* ----------------------------------------------------------------- */
/* static storage used by a() -- lazy initialisation                */
static seq_t          *seq       = NULL;    /* output sequence    */
static size_t          seq_len   = 0;       /* number of elements  */
static size_t          seq_cap   = 0;       /* allocated capacity */

static uint32_t       *spf       = NULL;    /* smallest prime factor array   */
static uint32_t       *prime1    = NULL;    /* primes ≡ 1 (mod 4)            */
static size_t          prime1_cnt= 0;
static uint32_t       *ks        = NULL;    /* numbers with no prime ≡1 (mod 4) */
static size_t          ks_cnt    = 0;

static Node           *heap      = NULL;    /* min‑heap of candidate products */
static size_t          heap_len  = 0;       /* current heap size              */

/* ----------------------------------------------------------------- */
/* minimal heap operations ------------------------------------------ */
static inline void heap_swap(Node *a, Node *b) {
    Node tmp = *a; *a = *b; *b = tmp;
}

static inline void heap_push(Node *h, size_t *len, Node n) {
    size_t i = (*len)++;
    while (i > 0) {
        size_t p = (i-1) >> 1;
        if (h[p].val <= n.val) break;
        h[i] = h[p];
        i   = p;
    }
    h[i] = n;
}

static inline Node heap_pop(Node *h, size_t *len) {
    Node ret = h[0];
    size_t last = --(*len);
    if (last > 0) {
        Node x = h[last];
        size_t i = 0;
        while (true) {
            size_t l = (i << 1) + 1;
            if (l >= last) break;
            size_t r = l + 1;
            size_t c = (r < last && h[r].val < h[l].val) ? r : l;
            if (h[c].val >= x.val) break;
            h[i] = h[c];
            i    = c;
        }
        h[i] = x;
    }
    return ret;
}

/* ----------------------------------------------------------------- */
/* auxiliary routine – linear sieve computing spf up to LIMIT -------- */
static void build_spf(uint32_t limit, uint32_t *spf_arr, uint32_t **primes_out, size_t *primes_cnt_out) {
    /* spf_arr assumed allocated with size limit+1 */
    spf_arr[0] = 0; spf_arr[1] = 1;
    uint32_t *primes = (uint32_t*)malloc(limit * sizeof(uint32_t));
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf_arr[i] && (uint64_t)i * primes[j] <= limit; ++j)
            spf_arr[i * primes[j]] = primes[j];
    }
    *primes_out = primes;
    *primes_cnt_out = primes_cnt;
}

/* ----------------------------------------------------------------- */
/* main function ----------------------------------------------------- */
seq_t a(idx_t n) {
    /* ----------------------------------------------------------- */
    /* lazy initialisation – executed only on the first call     */
    /* ----------------------------------------------------------- */
    if (seq == NULL) {
        /* 1. choose an upper bound that contains enough terms for
           n up to ~10000. 10 000 000 gives more than enough pieces
           while staying comfortably within typical memory limits.  */
        const uint32_t LIMIT = 10000000U;          /* 10 million            */
        /* allocate memory for spf */
        spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
        /* build spf and prime list */
        uint32_t *primes = NULL;
        size_t primes_cnt = 0;
        build_spf(LIMIT, spf, &primes, &primes_cnt);
        /* split primes into those ≡1 (mod 4) and the rest */
        size_t pcnt1 = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
            if (primes[i] % 4 == 1) ++pcnt1;
        prime1 = (uint32_t*)malloc(pcnt1 * sizeof(uint32_t));
        prime1_cnt = pcnt1;
        pcnt1 = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
            if (primes[i] % 4 == 1) prime1[pcnt1++] = primes[i];
        free(primes);
        /* 2. build ks: numbers <= LIMIT whose all prime divisors are 2 or ≡3 (mod 4) */
        ks = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t)); /* use as array of values */
        ks_cnt = 0;
        for (uint32_t i = 1; i <= LIMIT; ++i) {
            bool ok = true;
            uint32_t x = i;
            while (x > 1) {
                uint32_t p = spf[x];
                if ((p & 3) == 1) { ok = false; break; }
                while (x % p == 0) x /= p;
            }
            if (ok) ks[ks_cnt++] = i;
        }
        /* 3. initialise heap – one entry per prime ≡1 (mod 4) */
        heap = (Node*)malloc(prime1_cnt * sizeof(Node));
        heap_len = 0;
        for (size_t pi = 0; pi < prime1_cnt; ++pi) {
            Node node;
            node.pk = pi;
            node.ki = 0;                          /* start with k = ks[0] == 1 */
            node.val = (uint64_t)prime1[pi] * (uint64_t)prime1[pi] * (uint64_t)ks[0];
            heap_push(heap, &heap_len, node);
        }
        /* 4. initialise output array with a modest capacity */
        seq_cap = 1024;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* ----------------------------------------------------------- */
    /* generate terms until we have at least n of them              */
    /* ----------------------------------------------------------- */
    while (seq_len < (size_t)n) {
        /* pop smallest candidate */
        Node node = heap_pop(heap, &heap_len);
        seq[seq_len++] = (seq_t)node.val;
        /* advance the pointer for this prime */
        ++node.ki;
        if (node.ki < ks_cnt) {
            node.val = (uint64_t)prime1[node.pk] * (uint64_t)prime1[node.pk] * (uint64_t)ks[node.ki];
            heap_push(heap, &heap_len, node);
        }
    }

    /* ----------------------------------------------------------- */
    /* return the requested element (1‑based indexing)              */
    /* ----------------------------------------------------------- */
    return seq[(size_t)(n - 1)];
}
