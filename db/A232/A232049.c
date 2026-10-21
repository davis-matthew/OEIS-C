/*
 * A232049  Number of 3Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A232049: Number of 3×n 0..1 arrays with no element less than a strict
 *  majority of its horizontal and antidiagonal neighbors.
 *
 *  Empirical linear recurrence (valid for n>18):
 *      a(n) = 8*a(n-1) -27*a(n-2) +64*a(n-3) -104*a(n-4)
 *          +124*a(n-5) -102*a(n-6) +34*a(n-7) +10*a(n-8)
 *          -34*a(n-9) +19*a(n-10) -2*a(n-11) +a(n-12)
 *          +10*a(n-13) -2*a(n-14) -2*a(n-15)
 *
 *  Initial values (offset 1):
 *      a(1)=8,  a(2)=21,  a(3)=80,  a(4)=318,  a(5)=1315,
 *      a(6)=5364, a(7)=21680, a(8)=87452, a(9)=352931,
 *      a(10)=1425261, a(11)=5757064, a(12)=23253889,
 *      a(13)=93921193, a(14)=379334811, a(15)=1532086339,
 *      a(16)=6187939435, a(17)=24992490166, a(18)=100942222266
 *
 *  The implementation maintains a static cache for n ≤ 10000 and
 *  uses a sliding‑window recurrence for larger n.  All arithmetic
 *  is performed on the type seq_t; no floating point or external
 *  libraries are required.
 */

#include <stddef.h>


static const seq_t INIT[19] = {
    0ULL,                         /* index 0 unused */
    8ULL,         /* a(1) */
    21ULL,        /* a(2) */
    80ULL,        /* a(3) */
    318ULL,       /* a(4) */
    1315ULL,      /* a(5) */
    5364ULL,      /* a(6) */
    21680ULL,     /* a(7) */
    87452ULL,     /* a(8) */
    352931ULL,    /* a(9) */
    1425261ULL,   /* a(10) */
    5757064ULL,   /* a(11) */
    23253889ULL,  /* a(12) */
    93921193ULL,  /* a(13) */
    379334811ULL, /* a(14) */
    1532086339ULL,/* a(15) */
    6187939435ULL,/* a(16) */
    24992490166ULL,/* a(17) */
    100942222266ULL /* a(18) */
};

#define MAX_STATIC 10000

/* Static cache for up to MAX_STATIC terms */
static seq_t static_cache[MAX_STATIC + 1] = {0};
static size_t static_computed = 0;


/* Return the n-th term of sequence A232049.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)        /* not defined, return 0 for safety */
        return 0;

    /* ------------------------------------------------------------------
     * Ensure that the initial 18 values are present in the static cache.
     * ------------------------------------------------------------------ */
    if (static_computed == 0) {
        size_t i;
        for (i = 1; i <= 18 && i < sizeof(static_cache) / sizeof(static_cache[0]); ++i)
            static_cache[i] = INIT[i];
        static_computed = 18;
    }

    /* ------------------------------------------------------------------
     * Fast path: n within the static cache range.
     * ------------------------------------------------------------------ */
    if (n <= MAX_STATIC) {
        if (n > static_computed) {
            size_t i;
            for (i = static_computed + 1; i <= n; ++i) {
                /* Recurrence using only addition and subtraction of
                   non‑negative terms to avoid signed overflow.          */
                seq_t pos = 0, neg = 0;
                pos += 8ULL  * static_cache[i-1];
                pos += 64ULL * static_cache[i-3];
                pos += 124ULL * static_cache[i-5];
                pos += 34ULL  * static_cache[i-7];
                pos += 10ULL  * static_cache[i-8];
                pos += 19ULL  * static_cache[i-10];
                pos += 1ULL   * static_cache[i-12];
                pos += 10ULL  * static_cache[i-13];

                neg += 27ULL  * static_cache[i-2];
                neg += 104ULL * static_cache[i-4];
                neg += 102ULL * static_cache[i-6];
                neg += 34ULL  * static_cache[i-9];
                neg += 2ULL   * static_cache[i-11];
                neg += 2ULL   * static_cache[i-14];
                neg += 2ULL   * static_cache[i-15];

                static_cache[i] = pos - neg;
            }
            static_computed = n;
        }
        return static_cache[n];
    }

    /* ------------------------------------------------------------------
     * For n > MAX_STATIC we compute the value on the fly using a
     * sliding window of the last 15 terms.  We do not store the full
     * sequence because the values grow very rapidly and memory would be
     * wasteful.
     * ------------------------------------------------------------------ */
    {
        seq_t window[15];
        size_t j;

        /* Initialize window with a(18) … a(4) */
        for (j = 0; j < 15; ++j)          /* j = 0 → a(18), j = 14 → a(4) */
            window[j] = INIT[18 - j];

        seq_t current = 0;          /* will hold the freshly computed term */

        for (size_t i = 19; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            pos += 8ULL  * window[0];
            pos += 64ULL * window[2];
            pos += 124ULL * window[4];
            pos += 34ULL  * window[6];
            pos += 10ULL  * window[7];
            pos += 19ULL  * window[9];
            pos += 1ULL   * window[11];
            pos += 10ULL  * window[12];

            neg += 27ULL  * window[1];
            neg += 104ULL * window[3];
            neg += 102ULL * window[5];
            neg += 34ULL  * window[8];
            neg += 2ULL   * window[10];
            neg += 2ULL   * window[13];
            neg += 2ULL   * window[14];

            current = pos - neg;   /* a(i) */

            /* Shift window to make room for the next value */
            for (j = 14; j > 0; --j)
                window[j] = window[j-1];
            window[0] = current;
        }
        return current;
    }
}
