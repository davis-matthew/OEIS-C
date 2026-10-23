/*
 * A100771  Distance from the nearest palindrome.
 * https://oeis.org/A100771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = min(|n - p_left|, |n - p_right|) where p_left and p_right
 *            * are the nearest palindromic numbers less than or equal to n
 *            and greater * than or equal to n respectively
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   digits, make_pal_from_leftHalf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/make_pal_from_leftHalf.h"

/* Powers of ten.  POW10[i] == 10^i for i=0..20.  The largest needed value
 * is 10^19 which fits in unsigned long long. */
static const unsigned long long POW10[21] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL,
    100000000000000000000ULL
};

seq_t a(idx_t n)
{
    /* All single-digit numbers are palindromes. */
    if (n < 10ULL) return 0;

    int len = digits(n);               /* number of decimal digits of n */
    int leftHalfLen = (len + 1) / 2;   /* digits taken for the left side */

    /* divisor = 10^(len - leftHalfLen) */
    idx_t divisor = (idx_t)POW10[len - leftHalfLen];
    seq_t leftHalf = n / divisor;      /* leading leftHalfLen digits of n */

    seq_t p1 = make_pal_from_leftHalf(leftHalf, leftHalfLen, len);
    if (p1 == n) return 0;             /* n itself is a palindrome */

    seq_t leftCandidate, rightCandidate;

    if (p1 > n) { /* n lies between p1 and the previous palindrome */
        rightCandidate = p1;

        seq_t decLeft = leftHalf - 1;
        seq_t threshold = (seq_t)POW10[leftHalfLen - 1];
        if (decLeft < threshold) {
            /* Decrementing causes loss of a digit – the previous palindrome
             * is the largest number with len-1 digits, i.e. all 9's. */
            leftCandidate = (seq_t)(POW10[len - 1] - 1ULL);
        } else {
            leftCandidate = make_pal_from_leftHalf(decLeft, leftHalfLen, len);
        }
    } else { /* p1 < n */
        leftCandidate = p1;

        seq_t incLeft = leftHalf + 1;
        seq_t maxLeft = (seq_t)(POW10[leftHalfLen] - 1ULL);
        if (incLeft > maxLeft) {
            /* Incrementing causes a carry that increases the length.
             * The nearest larger palindrome is 1 followed by len-1 zeros and 1. */
            seq_t pal = 1;
            for (int i = 0; i < len - 1; ++i) pal *= 10ULL;
            pal = pal * 10ULL + 1ULL;    /* 1 0…0 1 */
            rightCandidate = pal;
        } else {
            rightCandidate = make_pal_from_leftHalf(incLeft, leftHalfLen, len);
        }
    }

    /* Compute absolute differences.  Everything is unsigned, so we use
     * conditional subtraction. */
    seq_t distLeft, distRight;
    if (n >= (idx_t)leftCandidate)
        distLeft = (seq_t)(n - (idx_t)leftCandidate);
    else
        distLeft = (seq_t)(((idx_t)leftCandidate) - n);

    if (n >= (idx_t)rightCandidate)
        distRight = (seq_t)(n - (idx_t)rightCandidate);
    else
        distRight = (seq_t)(((idx_t)rightCandidate) - n);

    return (distLeft < distRight) ? distLeft : distRight;
}
