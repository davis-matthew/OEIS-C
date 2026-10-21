/*
 * A063557  Smallest k such that 3^k has exactly n 2's in its decimal
 *          representation.
 * https://oeis.org/A063557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest k such that 3^k contains exactly n digits '2'
   in its decimal representation.  The computation is performed by
   successively multiplying by 3 in a decimal array and recording the
   first exponent that yields each count of twos. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the decimal representation of 3^k          */
    static uint8_t *digits = NULL;            /* least‑significant digit first */
    static size_t digits_len = 0;
    static size_t digits_cap = 0;

    /* static storage for the memoised answers a(m) = minimal k     */
    static idx_t *ans = NULL;                 /* ans[m] is the answer for m   */
    static size_t ans_len = 0;                /* current size of ans         */
    static size_t ans_cap = 0;

    /* current exponent (k) being stored in digits                    */
    static idx_t current_k = 0;

    /* --- initialization on first call --------------------------- */
    if (!digits) {
        /* start with 3^1 = 3 */
        digits_len = 1;
        digits_cap = 1;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 3;
        current_k = 1;

        /* a(0) = 1 because 3^1 has no digit '2' */
        ans_len = 1;
        ans_cap = 1;
        ans = (idx_t *)malloc(ans_cap * sizeof(idx_t));
        ans[0] = 1;
    }

    /* --- extend memoisation until we have a(n) ---------------- */
    while ((ans_len <= n) || (ans[n] == 0)) {

        /* multiply current number by 3 -------------------------------- */
        int carry = 0;
        size_t i;
        for (i = 0; i < digits_len; ++i) {
            int prod = digits[i] * 3 + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap = (digits_cap ? digits_cap * 2 : 1);
                digits = (uint8_t *)realloc(digits, digits_cap);
            }
            digits[digits_len++] = carry % 10;
            carry /= 10;
        }
        ++current_k;

        /* count number of digit '2' in the new power  */
        size_t cnt2 = 0;
        for (i = 0; i < digits_len; ++i)
            if (digits[i] == 2)
                ++cnt2;

        /* resize ans vector if necessary */
        if (cnt2 >= ans_len) {
            while (ans_len <= cnt2) {
                size_t new_cap = (ans_cap ? ans_cap * 2 : 1);
                if (new_cap <= ans_len) new_cap = ans_len + 1;
                ans = (idx_t *)realloc(ans, new_cap * sizeof(idx_t));
                while (ans_len < new_cap) ans[ans_len++] = 0;
                ans_cap = new_cap;
            }
        }

        /* store first occurrence */
        if (ans[cnt2] == 0)
            ans[cnt2] = current_k;
    }

    return (seq_t)ans[n];
}
