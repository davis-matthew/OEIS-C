/*
 * A211346  a(1) = 1; for n>1, let k = n-1 mod 4, then a(n) = operation k
 *          applied to the pair (a(n-1), n), where operation 1 is addition,
 *          operation 2 is difference (with reset to n if 0), operation 3 is
 *          multiplication and operation 0 is division with rounding.
 * https://oeis.org/A211346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 947, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1; for n>1 let k=(n-1)%4. Then
   k==1:  a(n) = a(n-1)+n;
   k==2:  a(n) = (a(n-1)==n) ? n : |a(n-1)-n|;
   k==3:  a(n) = a(n-1)*n;
   k==0:  a(n) = round(a(n-1)/n);            // rounding to nearest integer */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t prev = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        idx_t k = (i - 1) % 4;
        seq_t val = (seq_t)i;

        switch (k) {
            case 1: /* addition */
                prev = prev + val;
                break;
            case 2: /* difference with reset */
                if (prev == val)
                    prev = val;
                else
                    prev = (prev > val) ? (prev - val) : (val - prev);
                break;
            case 3: /* multiplication */
                prev = prev * val;
                break;
            case 0: /* division with rounding */
                prev = (prev + (val / 2)) / val;
                break;
        }
    }

    return prev;
}
