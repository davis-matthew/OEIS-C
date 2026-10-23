/*
 * A147647  Nine-digit numbers with all distinct digits such that the string
 *          formed by the last k digits is divisible by k for any 1<=k<=9.
 * https://oeis.org/A147647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   Computes a(n) by exhaustive search: uses recursion to generate
 *            all 9‑digit numbers with distinct digits where the number formed
 *            by the last k digits is divisible by k for each k = 1..9. All
 *            202 such numbers are generated, sorted, and returned by index
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The build system defines seq_t (integer or big‑int type) and idx_t. */

static const uint32_t POW10[9] = {
    1u, 10u, 100u, 1000u, 10000u,
    100000u, 1000000u, 10000000u, 100000000u
};

static uint8_t digits[9];        /* digits[0] = least significant */
static seq_t cache[202];         /* all valid numbers, sorted */
static unsigned int cache_len = 0;
static int built = 0;

/* Recursive enumeration of admissible numbers.
   pos   – current digit position (0 = least significant).
   suffix – value of the digits already chosen (positions 0..pos‑1).
   used  – bitmask of digits already used. */
static void rec(int pos, uint32_t suffix, uint16_t used)
{
    if (pos == 9) {
        /* assemble the 9‑digit number */
        uint32_t num = 0;
        for (int i = 0; i < 9; ++i)
            num += digits[i] * POW10[i];
        cache[cache_len++] = (seq_t)num;
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        if (used & (1u << d)) continue;          /* digit already used   */
        if (pos == 8 && d == 0) continue;         /* leading zero forbidden */

        uint32_t new_suffix = suffix + (uint32_t)d * POW10[pos];
        int k = pos + 1;
        if (k > 1 && new_suffix % k != 0) continue;

        digits[pos] = (uint8_t)d;
        rec(pos + 1, new_suffix, used | (1u << d));
    }
}

/* Simple insertion sort for the small array. */
static void sort_cache(void)
{
    for (unsigned int i = 1; i < cache_len; ++i) {
        unsigned int j = i;
        while (j > 0 && cache[j - 1] > cache[j]) {
            seq_t tmp = cache[j - 1];
            cache[j - 1] = cache[j];
            cache[j] = tmp;
            --j;
        }
    }
}

/* External interface */
seq_t a(idx_t n)
{
    if (!built) {
        rec(0, 0, 0);
        sort_cache();
        built = 1;
    }
    if (n == 0) return 0;                       /* undefined, safety */
    if ((idx_t)cache_len >= n) return cache[n - 1];
    return 0;                                   /* out of bounds */
}
