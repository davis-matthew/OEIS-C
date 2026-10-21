/*
 * A072175  a(1)=1, a(2)=2, a(n) = a(n-1) + 1 - 2*sign(a(n-2)) for n>2.
 * https://oeis.org/A072175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A072175
 * Recurrence:  a(0)=1, a(1)=2, a(2)=1,
 *              a(n)=a(n-1)+1-2*sign(a(n-2)) for n>2
 * It is observed that for n≥1 the sequence is periodic with period 8:
 *   a(1)= 2, a(2)= 1, a(3)= 0, a(4)=-1, a(5)= 0, a(6)= 3, a(7)= 4, a(8)= 3
 * and repeats.  This direct pattern yields an O(1) computation for all n.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* period 8 for n>=1 */
    static const seq_t period[8] = {
        (seq_t)2,  /* n = 1 */
        (seq_t)1,  /* n = 2 */
        (seq_t)0,  /* n = 3 */
        (seq_t)(-1),/* n = 4 */
        (seq_t)0,  /* n = 5 */
        (seq_t)3,  /* n = 6 */
        (seq_t)4,  /* n = 7 */
        (seq_t)3   /* n = 8 */
    };

    /* n-1 is safe because we already handled n==0 */
    return period[(n - 1) % 8];
}
