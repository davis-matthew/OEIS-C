/*
 * A035074  a(n) is root of square starting with digit 7: first term of runs.
 * https://oeis.org/A035074
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(sqrt(7 * 10^n)) (n > 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the digit‑by‑digit extraction of the square root of the
   integer 7·10^n.  The algorithm works for any seq_t that supports
   the basic arithmetic operations listed in the requirements.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2 only.  */
    if (n < 2)
        return 0;

    /* Total number of decimal digits of 7*10^n : '7' followed by n zeros. */
    idx_t totalDigits = n + 1;

    seq_t root = 0;          /* current root (integer part)          */
    seq_t rem  = 0;          /* remainder in the extraction process   */

    /* First group: 1 digit if the total length is odd, else 2 digits. */
    seq_t firstGroup = (totalDigits % 2 == 0) ? (seq_t)70 : (seq_t)7;
    rem = firstGroup;

    /* Find the first digit d such that d^2 ≤ rem.  */
    idx_t d;
    for (d = 9; d > 0; --d) {
        seq_t sq = (seq_t)d * (seq_t)d;
        if (sq <= rem) {
            root = (seq_t)d;
            rem -= sq;
            break;
        }
    }
    /* If rem was 0 we would have d == 0; in that case root remains 0. */

    /* Total number of groups (first group may contain 1 or 2 digits). */
    idx_t groups = (totalDigits + 1) / 2;

    /* Process remaining groups (all of them are "00" in this case). */
    for (idx_t g = 1; g < groups; ++g) {
        rem = rem * 100;              /* bring down the next pair of zeros */

        seq_t left = root * 20;       /* 20 * current root */

        /* Find the largest d with (left + d) * d ≤ rem. */
        for (d = 9; d > 0; --d) {
            seq_t cand = (left + (seq_t)d) * (seq_t)d;
            if (cand <= rem) {
                root = root * 10 + (seq_t)d;
                rem -= cand;
                break;
            }
        }
        if (d == 0) {                /* when rem < left, the next digit is 0 */
            root = root * 10;
        }
    }

    /* Since 7·10^n is never a perfect square, ceil = floor + 1. */
    return root + 1;
}
