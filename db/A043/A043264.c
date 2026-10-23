/*
 * A043264  Sum of the digits of the n-th base 5 palindrome.
 * https://oeis.org/A043264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = sum of the digits of the n-th base‑5 palindrome
 *   (palindromes are listed in increasing order, starting with 0,
 *    with the first non‑zero digit non‑zero).
 *
 * The algorithm:
 * 1. Determine the length L of the palindrome for the given index n.
 * 2. Compute the offset of n within the L‑digit palindromes.
 * 3. Decode the offset into the left half of the palindrome
 *    (including the middle digit for odd L); sum the digits.
 * 4. For even L the total digit sum is 2*(sum_of_left_half).
 *    For odd L the total digit sum is 2*(sum_of_left_excluding_middle)
 *    plus the middle digit.
 *
 * The function runs in time O(L) (L ≤ 11 for n ≤ 10000) and
 * uses only integer arithmetic.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t pow_u64(idx_t base, idx_t exp) {
    idx_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* number of L‑digit base‑5 palindromes (first digit non‑zero except L==1) */
static idx_t count_len(idx_t len) {
    const idx_t base = 5;
    if (len == 0)
        return 0;
    if (len == 1)
        return 5; /* 0,1,2,3,4 */
    if (len % 2 == 0) {                /* even length 2k */
        idx_t k = len / 2;
        return (base - 1) * pow_u64(base, k - 1);
    } else {                           /* odd length 2k+1 */
        idx_t k = (len - 1) / 2;
        return (base - 1) * pow_u64(base, k);
    }
}

seq_t a(idx_t n) {
    const idx_t base = 5;
    if (n <= 1)          /* includes n == 0, which is out of sequence range */
        return (seq_t)(n - 1);

    /* Find the length of the required palindrome */
    idx_t len = 1;
    idx_t rem = n;                /* remaining index to locate */
    while (1) {
        idx_t cnt = count_len(len);
        if (rem <= cnt)
            break;
        rem -= cnt;
        ++len;
    }

    /* offset within the chosen length group (0‑based) */
    idx_t offset = rem - 1;

    if (len == 1) {                     /* single digit: value equals offset */
        return (seq_t)offset;
    }

    if (len % 2 == 0) {                 /* even length 2k */
        idx_t k = len / 2;
        idx_t pow_half_minus_1 = pow_u64(base, k - 1);

        idx_t first_index = offset / pow_half_minus_1;      /* 0..3 */
        seq_t first_digit = (seq_t)(first_index + 1);        /* 1..4 */
        idx_t remainder = offset % pow_half_minus_1;        /* k-1 digits */

        seq_t sum_left = first_digit;
        for (idx_t i = 0; i < k - 1; ++i) {
            seq_t d = (seq_t)(remainder % base);
            remainder /= base;
            sum_left += d;
        }
        return sum_left * 2;
    } else {                            /* odd length 2k+1 */
        idx_t k = len / 2;               /* floor(len/2) */
        idx_t pow_half = pow_u64(base, k);

        idx_t first_index = offset / pow_half;             /* 0..3 */
        seq_t first_digit = (seq_t)(first_index + 1);       /* 1..4 */
        idx_t remainder = offset % pow_half;               /* k digits */

        seq_t middle = (seq_t)(remainder % base);
        remainder /= base; /* remove middle digit */

        seq_t sum_left = first_digit;
        for (idx_t i = 0; i < k - 1; ++i) {
            seq_t d = (seq_t)(remainder % base);
            remainder /= base;
            sum_left += d;
        }
        return sum_left * 2 + middle;
    }
}
