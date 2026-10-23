/*
 * A100467  Semiprimes of special form: sum of a semiprime k and the k-th
 *          semiprime.
 * https://oeis.org/A100467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = semiprime(k) + semiprime_k, where k is a semiprime
   and the sum is itself a semiprime.  This implementation precomputes
   all semiprimes up to a fixed bound and generates the sequence by
   checking each semiprime k.  The bound is chosen large enough to
   produce at least 10 000 terms. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---- constants ------------------------------------------------------- */

#define MAXV       20000000          /* sieve limit */
#define INIT_CAP   3000000           /* initial capacity for semiprime array */
#define SEQ_INIT   200000            /* initial capacity for sequence array */

/* ---- global data ------------------------------------------------------ */

static uint32_t *spf      = NULL;   /* smallest prime factor */
static uint32_t *primes   = NULL;   /* primes used in linear sieve */
static size_t   primes_cap = 0;
static size_t   primes_sz  = 0;

static uint32_t *semip    = NULL;   /* list of semiprimes */
static size_t   semip_cap = 0;
static size_t   semip_sz  = 0;

static seq_t   *seq      = NULL;    /* sequence values */
static size_t   seq_cap   = 0;
static size_t   seq_sz    = 0;

/* ---- helpers ----------------------------------------------------------- */

static inline bool is_semiprime(uint32_t n) {
    uint32_t p = spf[n];
    uint32_t q = n / p;
    return (q >= 2U && spf[q] == q);
}

/* ---- initialization ---------------------------------------------------- */

static void init(void) {
    if (spf != NULL) return;   /* already initialized */

    /* allocate spf array */
    spf = (uint32_t *)calloc(MAXV + 1, sizeof(uint32_t));
    if (!spf) return;   /* allocation failed; handled by caller */

    /* linear sieve */
    primes_cap = 1000000;
    primes = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));
    for (uint32_t i = 2; i <= MAXV; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_sz == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_sz++] = i;
        }
        for (size_t j = 0; j < primes_sz; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > spf[i] || prod > MAXV) break;
            spf[prod] = p;
        }
    }
    /* free primes array - not needed after sieve */
    free(primes); primes = NULL; primes_cap = primes_sz = 0;

    /* semiprime list */
    semip_cap = INIT_CAP;
    semip = (uint32_t *)malloc(semip_cap * sizeof(uint32_t));
    for (uint32_t i = 4; i <= MAXV; ++i) {
        if (is_semiprime(i)) {
            if (semip_sz == semip_cap) {
                semip_cap *= 2;
                semip = (uint32_t *)realloc(semip, semip_cap * sizeof(uint32_t));
            }
            semip[semip_sz++] = i;
        }
    }

    /* build sequence values */
    seq_cap = SEQ_INIT;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    const size_t total = semip_sz;
    for (size_t idx = 0; idx < total; ++idx) {
        uint32_t k_val = semip[idx];
        if (k_val == 0 || k_val > total) continue;   /* index out of bounds */
        uint32_t s_k   = semip[k_val - 1];
        uint32_t sum   = k_val + s_k;
        if (sum <= MAXV && is_semiprime(sum)) {
            if (seq_sz == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_sz++] = (seq_t)sum;
        }
    }

    /* sort and deduplicate the sequence */
    qsort(seq, seq_sz, sizeof(seq_t), (int(*)(const void*,const void*))((void(*)(void))memcmp));
    size_t uniq = 0;
    for (size_t i = 0; i < seq_sz; ++i) {
        if (uniq == 0 || seq[i] != seq[uniq-1]) {
            seq[uniq++] = seq[i];
        }
    }
    seq_sz = uniq;
}

/* ---- sequence function ------------------------------------------------- */

seq_t a(idx_t n) {
    init();
    if (n == 0) return 0;
    size_t i = (size_t)(n - 1);
    if (i < seq_sz) return seq[i];
    /* index beyond precomputed range – undefined, return 0 */
    return 0;
}
