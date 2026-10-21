/*
 * A060108  Sequence of sums based on primes = 7 mod 8.
 * https://oeis.org/A060108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p^2 - 1) / 24 where p is the n-th prime congruent to 7
 *            (mod 8) (OEIS A007522)
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* ---------- Helper: Growing prime list for primality tests ----------
 * We keep a dynamic array of all primes found so far.  This is used to
 * test new candidates by trial division.  Only odd candidates are
 * examined after the initial prime 2.  The array grows as needed.
 */
static uint64_t *all_primes = NULL;        /* array of all primes seen   */
static size_t     all_primes_cnt = 0;      /* number of primes stored   */
static size_t     all_primes_cap = 0;      /* allocated capacity        */
static uint64_t   next_candidate = 3;      /* next odd candidate tested */

/* Append a new prime to the global prime list, growing if necessary. */
static void append_prime(uint64_t p)
{
    if (all_primes_cnt == all_primes_cap) {
        size_t new_cap = all_primes_cap ? all_primes_cap << 1 : 32;
        uint64_t *tmp = (uint64_t*)realloc(all_primes, new_cap * sizeof(uint64_t));
        all_primes = tmp;
        all_primes_cap = new_cap;
    }
    all_primes[all_primes_cnt++] = p;
}

/* Trial‑division test of cand using the list of primes already known. */
static int is_prime(uint64_t cand)
{
    for (size_t i = 0; i < all_primes_cnt; ++i) {
        uint64_t p = all_primes[i];
        if (p > cand / p) break;   /* p*p > cand  */
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* ---------- Implementation of the sequence generator ----------
 * We maintain a dynamic array of primes ≡ 7 (mod 8).  The function
 * ensure_primes7() guarantees that this array contains at least `need`
 * such primes, generating new ones as necessary.
 */
static uint64_t *primes7 = NULL;      /* array of primes ≡7 mod 8       */
static size_t    primes7_cnt = 0;     /* number of stored primes       */
static size_t    primes7_cap = 0;     /* array capacity                */

/* Ensure that we have at least `need` primes congruent to 7 (mod 8). */
static void ensure_primes7(idx_t need)
{
    /* Initialise all_primes on the first call. */
    if (all_primes == NULL) {
        size_t cap = 32;
        all_primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
        all_primes_cap = cap;
        all_primes_cnt = 0;
        append_prime(2);                 /* first prime */
    }

    /* Initialise primes7 array once. */
    if (primes7 == NULL) {
        primes7_cap = 32;
        primes7 = (uint64_t*)malloc(primes7_cap * sizeof(uint64_t));
        primes7_cnt = 0;
    }

    while (primes7_cnt < need) {
        uint64_t cand = next_candidate;

        /* Test cand for primality. */
        if (is_prime(cand)) {
            append_prime(cand);                /* add to global list */

            if ((cand & 7ULL) == 7ULL) {       /* cand ≡ 7 (mod 8) */
                if (primes7_cnt == primes7_cap) {
                    size_t new_cap = primes7_cap << 1;
                    uint64_t *tmp = (uint64_t*)realloc(primes7, new_cap * sizeof(uint64_t));
                    primes7 = tmp;
                    primes7_cap = new_cap;
                }
                primes7[primes7_cnt++] = cand;
            }
        }

        /* Advance to the next odd candidate. */
        next_candidate = cand + 2;
    }
}

/* ---------- Sequences function ----------
 * The function has external linkage and follows all restrictions of
 * the exercise.  It works for any seq_t type that supports the
 * required arithmetic operators.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused but safe */

    ensure_primes7(n);

    seq_t p = (seq_t)primes7[n - 1];      /* nth prime ≡ 7 mod 8 */
    seq_t sq = p * p;                     /* p^2          */
    seq_t res = (sq - (seq_t)1) / (seq_t)24;
    return res;
}
