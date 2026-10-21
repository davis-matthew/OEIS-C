/*
 * A083985  If k is a number with exactly two distinct decimal digits, say a
 *          and b, neither of which is 0 (i.e., a member of A101594), define
 *          the self-complement of k, SC(k), to be the number obtained by
 *          replacing a with b and vice versa. Then a(n) = gcd(A101594(n),
 *          SC(A101594(n))).
 * https://oeis.org/A083985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd, has_two_nonzero_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = gcd(A101594(n), SC(A101594(n))),
 * where A101594(n) is the nth positive integer having exactly
 * two distinct non‑zero decimal digits and SC(k) is obtained by
 * swapping those two digits in k.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/has_two_nonzero_distinct_digits.h"

static seq_t self_complement(seq_t k)
{
    uint64_t x = (uint64_t)k;
    int a = 0, b = 0;
    int used[10] = {0};

    /* identify the two distinct digits */
    while (x > 0) {
        int d = (int)(x % 10);
        if (!used[d]) {
            used[d] = 1;
            if (a == 0)
                a = d;
            else
                b = d;
        }
        x /= 10;
    }

    /* rebuild the number with digits swapped */
    seq_t out = 0;
    seq_t pow10 = 1;
    x = (uint64_t)k;
    while (x > 0) {
        int d = (int)(x % 10);
        int nd = d;
        if (d == a)
            nd = b;
        else if (d == b)
            nd = a;
        out += (seq_t)nd * pow10;
        pow10 *= 10;
        x /= 10;
    }
    return out;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t cnt = 0;
    uint64_t num = 12; /* first number with two distinct non‑zero digits */

    while (1) {
        if (has_two_nonzero_distinct_digits(num)) {
            ++cnt;
            if (cnt == n) {
                seq_t k = (seq_t)num;
                seq_t kc = self_complement(k);
                return gcd(k, kc);
            }
        }
        ++num;
    }
}
