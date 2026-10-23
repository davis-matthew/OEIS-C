/*
 * A193405  The Matula numbers of the rooted trees that have a perfect
 *          matching.
 * https://oeis.org/A193405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS sequence A193405: the n-th Matula number of a
    rooted tree that has a perfect matching.  The implementation
    pre‑computes all Matula numbers up to a fixed bound, determines
    which trees admit a perfect matching by a simple Boolean DP,
    stores the matching Matula numbers in ascending order, and
    returns the requested element.  The bound is large enough to
    contain more than 10 000 terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXN 800000u            /* upper bound of Matula numbers to examine */

static int       sf[MAXN + 1];  /* smallest prime factor for each n */
static int       primes[MAXN];  /* list of primes up to MAXN */
static int       prime_cnt;
static int       pindex[MAXN + 1];   /* prime → its 1‑based index */

static bool      up[MAXN + 1];  /* matched to a child (root inside tree) */
static bool      down[MAXN + 1];/* matched to parent (root outside tree) */

/* array of resulting sequence values and its length */
static seq_t    *seq_vals = NULL;
static idx_t    seq_len   = 0;

static bool     initialized = false;

/* ------------------------------------------------------------------ */
/*  pre‑computation of smallest prime factors, prime list, and DP   */
/* ------------------------------------------------------------------ */
static void init(void)
{
    int i, j, p, n;

    /* small prime factor sieve */
    for (i = 0; i <= MAXN; ++i) sf[i] = i;
    for (i = 2; (long long)i * i <= MAXN; ++i) {
        if (sf[i] == i) {
            for (j = i * i; j <= MAXN; j += i)
                if (sf[j] == j) sf[j] = i;
        }
    }

    /* prime list and index mapping */
    prime_cnt = 0;
    for (i = 2; i <= MAXN; ++i)
        if (sf[i] == i) primes[prime_cnt++] = i;

    for (i = 0; i <= MAXN; ++i) pindex[i] = 0;
    for (i = 0; i < prime_cnt; ++i)
        pindex[primes[i]] = i + 1;      /* 1‑based indices */

    /* DP arrays */
    up[1]    = true;   /* single vertex: can be matched upwards */
    down[1]  = false;  /* cannot be matched to a child */
    seq_len  = 0;

    /* pre-allocate sequence buffer, one slot per n is safe */
    seq_vals = (seq_t *)malloc(MAXN * sizeof(seq_t));

    /* iterate all Matula numbers */
    for (n = 2; n <= MAXN; ++n) {
        int temp = n;
        bool allDown = true, anyUp = false;
        int bad_count = 0, bad_child = -1;

        while (temp > 1) {
            int p = sf[temp];
            int child = pindex[p];
            do {
                if (!down[child]) { bad_count++; bad_child = child; }
                if (up[child])    { anyUp = true; }
                allDown &= down[child];
                temp /= p;
            } while (temp % p == 0);
        }
        up[n] = allDown;

        if (bad_count >= 2)          down[n] = false;
        else if (bad_count == 0)     down[n] = anyUp;
        else /* bad_count==1 */       down[n] = up[bad_child];

        if (down[n])                 seq_vals[seq_len++] = (seq_t)n;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  public interface                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0; /* undefined, return 0 */
    if ((idx_t)(n - 1) >= seq_len) return 0; /* out of pre‑computed range */

    return seq_vals[(size_t)(n - 1)];
}
