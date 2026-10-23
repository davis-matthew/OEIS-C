/*
 * A032940  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^(m-i) has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 748, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation has all zeros in odd positions.
   For a given index n (n ≥ 1) the function returns the n‑th such
   integer in increasing order.  It works for any seq_t (C unsigned
   integer or arbitrary‑precision) and idx_t (uint64_t). */

#include <stddef.h>
#include <stdint.h>

/* external types */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for index 0, return 0 */

    /* Find the length L of the base‑5 representation of the answer. */
    unsigned long long cum = 0;            /* number of terms up to length L-1 */
    unsigned L = 0;                        /* current length */

    /* unsigned long long can hold all counts needed for n up to tens of thousands */
    while (1) {
        L++;                               /* try length L */
        unsigned k = (L + 1) / 2;          /* number of even positions in this length */

        unsigned long long countLen = 4ULL; /* first digit can be 1..4 */
        for (unsigned i = 1; i < k; i++)   /* remaining even digits (k-1) */
            countLen *= 5ULL;

        if (cum + countLen >= n) break;    /* found the right length */
        cum += countLen;
    }

    /* 0‑based position of the term among all terms of length L */
    unsigned long long offset = n - cum - 1ULL;

    /* Compute leading digit and remaining even digits */
    unsigned k = (L + 1) / 2;              /* number of even positions */

    unsigned long long powHalf = 1ULL;     /* 5^(k-1) */
    for (unsigned i = 1; i < k; i++)
        powHalf *= 5ULL;

    unsigned leading_index = (unsigned)(offset / powHalf);   /* 0..3 -> digit 1..4 */
    unsigned leading_digit = leading_index + 1;

    unsigned long long rem = offset % powHalf;              /* remaining part */

    /* digits for the remaining (k‑1) even positions (from most to least significant) */
    unsigned digitsEven[10] = {0};   /* enough for k up to ~10 */
    for (int i = (int)(k - 2); i >= 0; i--) {
        digitsEven[i] = (unsigned)(rem % 5ULL);
        rem /= 5ULL;
    }

    /* Build the full number in base 5, inserting zeros at odd positions */
    seq_t val = 0;
    unsigned evenIdx = 0;   /* index in even positions */

    for (unsigned pos = 0; pos < L; pos++) {
        unsigned digit;
        if (pos % 2 == 0) {               /* even position */
            if (evenIdx == 0)
                digit = leading_digit;
            else
                digit = digitsEven[evenIdx - 1];
            evenIdx++;
        } else {                           /* odd position: must be 0 */
            digit = 0;
        }
        val = val * (seq_t)5 + (seq_t)digit;
    }

    return val;
}
