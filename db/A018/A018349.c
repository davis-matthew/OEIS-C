/*
 * A018349  Divisors of 238.
 * https://oeis.org/A018349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 238 in ascending order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Prime factorisation of 238 = 2 * 7 * 17 */
static const uint64_t primes[3] = {2ULL, 7ULL, 17ULL};
static const int     exps  [3] = {1,   1,    1};

/* Recursively generate all products of the prime powers. */
static void gen_divs(int idx, seq_t val, seq_t *list, int *cnt)
{
    if (idx == 3) {                 /* all primes processed */
        list[(*cnt)++] = val;
        return;
    }
    for (int i = 0; i <= exps[idx]; ++i) {  /* exponent for current prime */
        gen_divs(idx + 1, val, list, cnt);
        val *= (seq_t)primes[idx];
    }
}

/* Build the sorted array of all divisors of 238 (8 in total). */
static void build_divisors(seq_t *divs)
{
    int cnt = 0;
    gen_divs(0, (seq_t)1, divs, &cnt);

    /* Simple insertion sort – only 8 elements */
    for (int i = 1; i < cnt; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }
}

seq_t a(idx_t n)
{
    /* Lazy build to avoid unnecessary work. */
    static seq_t divs[8];
    static int   built = 0;
    if (!built) {
        build_divisors(divs);
        built = 1;
    }

    if (n >= 1 && n <= 8) {
        return divs[n - 1];
    }
    /* Undefined terms – return 0. */
    return (seq_t)0;
}
