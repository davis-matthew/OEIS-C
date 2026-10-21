/*
 * A084029  Triangle read by rows: n-th row contains the first n n-digit
 *          multiples of n with digit sum n. If there are fewer than n such
 *          numbers, the rest of the row is filled with 0's.
 * https://oeis.org/A084029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, tabl
 * Formula:   a(n) = the n‑th term of the triangle read by rows where the r‑th
 *            row contains the first r r‑digit multiples of r with digit sum
 *            r. The sequence is indexed with offset 1: a(1) is the first
 *            printed value
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   add_small_to_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/add_small_to_big.h"

/* Helper: return 10^(exp) mod mod,  exp>=0 */
static idx_t pow10_mod(idx_t mod, idx_t exp)
{
    if (mod == 1) return 0;
    idx_t r = 1 % mod;
    while (exp--) {
        r = (r * 10) % mod;
    }
    return r;
}

/* Helper: convert the big integer digits[] (least significant first) to seq_t value */
static seq_t digits_to_seq(const unsigned char *digits, size_t len)
{
    seq_t val = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        val = val * 10 + (seq_t)digits[i];
    return val;
}

/* Compute the n-th term of A084029 */
seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0) return (seq_t)0;          /* offset is 1, n>=1, but just in case */
    if (n == 1) return (seq_t)1;

    /* determine the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* 0‑based position inside the row */
    idx_t prevSum = (r - 1) * r / 2;
    idx_t pos = n - prevSum - 1;       /* pos in [0, r-1] */

    /* row 2 has only one number, so the second entry is 0 */
    if (r == 2) {
        if (pos == 0) return (seq_t)20;
        return (seq_t)0;
    }

    /* general case for r >= 3 */
    /* allocate array for digits (least significant first); maximum length r+1 */
    size_t cap = (size_t)r + 2;
    unsigned char *digits = (unsigned char*)malloc(cap);
    if (!digits) return (seq_t)0; /* allocation failure */

    /* initialize to 10^(r-1) */
    for (size_t i = 0; i < (size_t)r; ++i)
        digits[i] = 0;
    digits[(size_t)r - 1] = 1;
    size_t digits_len = (size_t)r;

    /* compute 10^(r-1) mod r */
    idx_t rem = pow10_mod(r, (idx_t)r - 1);
    idx_t offset = (r - rem) % r;            /* smallest offset to make a multiple of r */

    /* add offset to digits */
    int carry = (int)offset;
    size_t i = 0;
    while (carry > 0 && i < digits_len) {
        int sum = digits[i] + carry;
        digits[i] = (unsigned char)(sum % 10);
        carry = sum / 10;
        ++i;
    }
    while (carry > 0) {
        digits[digits_len++] = (unsigned char)(carry % 10);
        carry /= 10;
    }

    /* precompute digits of the small integer r for addition */
    unsigned char small_digits[10];
    size_t small_len = 0;
    idx_t tmp = r;
    do {
        small_digits[small_len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* search for the desired element */
    uint32_t found = 0;
    int digit_sum = 0;
    for (size_t k = 0; k < digits_len; ++k)
        digit_sum += digits[k];

    seq_t result = 0;
    while (1) {
        if (digit_sum == (int)r) {
            if (found == pos) {
                result = digits_to_seq(digits, digits_len);
                break;
            }
            ++found;
        }

        /* stop if number length would exceed r (not r-digit any more) */
        if (digits_len > (size_t)r) {
            result = (seq_t)0;
            break;
        }

        /* add r to the number */
        add_small_to_big(digits, &digits_len, small_digits, small_len);

        /* recompute digit sum */
        digit_sum = 0;
        for (size_t k = 0; k < digits_len; ++k)
            digit_sum += digits[k];
    }

    free(digits);
    return result;
}
