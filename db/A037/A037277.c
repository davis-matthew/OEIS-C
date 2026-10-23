/*
 * A037277  Replace n with concatenation of its divisors >1.
 * https://oeis.org/A037277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = concatenation of all divisors of n that are >1 (a(1)=0)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 35, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/*--------- helper to count decimal digits of a 64‑bit integer --------*/
static inline int count_digits(uint64_t x)
{
    int cnt = 0;
    do {
        ++cnt;
        x /= 10;
    } while (x);
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    uint64_t n64 = (uint64_t)n;
    /* a divisor >1; maximum number of divisors for n <= 10000 is < 200 */
    uint64_t divs[128];
    size_t cnt = 0;

    /* collect proper divisors (excluding 1) */
    for (uint64_t d = 2; d * d <= n64; ++d) {
        if (n64 % d == 0) {
            divs[cnt++] = d;
            if (d != n64 / d) {
                divs[cnt++] = n64 / d;
            }
        }
    }
    /* include n itself */
    divs[cnt++] = n64;

    /* simple insertion sort for the small list of divisors */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* concatenate the sorted divisors into a single number */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t d = divs[i];
        /* compute place shift = 10^(digits of d) */
        seq_t shift = 1;
        int dlen = count_digits(d);
        for (int k = 0; k < dlen; ++k) {
            shift *= (seq_t)10;
        }
        result = result * shift + (seq_t)d;
    }
    return result;
}
