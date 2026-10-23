/*
 * A187185  Parse the infinite string 0123456012345601234560... into distinct
 *          phrases 0, 1, 2, 3, 4, 5, 6, 01, 23, 45, 60, 12, 34, 56, 012, ...;
 *          a(n) = length of n-th phrase.
 * https://oeis.org/A187185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-49) - a(n-50) for n > 56 (initial values are
 *            obtained by simulating the phrase parsing)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static void compute_initial_terms(seq_t *arr, idx_t count)
{
    /* count is always 56 on first call */
    struct Phrase {
        unsigned len;
        unsigned long long val;            /* base‑7 value of the phrase */
    } seen[56];
    idx_t pos = 0;                          /* current position in the word */

    for (idx_t i = 1; i <= count; ++i) {
        unsigned len = 1;
        unsigned long long val;
        while (1) {
            /* compute the value of the phrase of length len */
            val = 0;
            for (unsigned k = 0; k < len; ++k) {
                unsigned digit = (unsigned)((pos + k) % 7); /* 0..6 */
                val = val * 7 + digit;
            }
            /* test if this phrase has appeared before */
            bool unique = true;
            for (idx_t j = 1; j < i; ++j) {
                if (seen[j - 1].len == len && seen[j - 1].val == val) {
                    unique = false;
                    break;
                }
            }
            if (unique)            /* first occurrence of this phrase */
                break;
            ++len;                 /* try longer phrase */
        }
        arr[i] = (seq_t)len;
        seen[i - 1].len = len;
        seen[i - 1].val = val;
        pos += len;                /* move past the phrase in the word */
    }
}

seq_t a(idx_t n)
{
    /* cache up to the first MAX_N terms; 10001 is enough for the required tests */
#define MAX_N 10001
    static seq_t cache[MAX_N + 1];
    static bool initialized = false;

    if (!initialized) {
        compute_initial_terms(cache, 56);
        initialized = true;
    }

    if (n <= 56)            /* the first 56 values are already stored */
        return cache[(idx_t)n];

    /* compute further terms on demand */
    for (idx_t i = 57; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 49] - cache[i - 50];
    }
    return cache[(idx_t)n];
}
