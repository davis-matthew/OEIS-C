/*
 * A073769  Duplicate of A008937.
 * https://oeis.org/A073769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=0, a(n)=a(n-1)+T(n) with tribonacci T(1)=1, T(2)=1, T(3)=2,
 *            T(k)=T(k-1)+T(k-2)+T(k-3) for k≥4 (i.e. a(n) is the partial sum
 *            of tribonacci numbers)
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    /* tribonacci values for indices 1,2,3 */
    seq_t t1 = (seq_t)1; /* T(1) */
    seq_t t2 = (seq_t)1; /* T(2) */
    seq_t t3 = (seq_t)2; /* T(3) */

    if (n >= 1) sum += t1;
    if (n >= 2) sum += t2;
    if (n >= 3) sum += t3;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = t3 + t2 + t1;          /* T(i) */
        sum += next;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return sum;
}
