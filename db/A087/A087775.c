/*
 * A087775  a[1] = 1, a[2] = 2, a[3] = 2; a[n] = 3*a[abs[a[n-2]]] -
 *          3*a[n-abs[a[n-2]]] + a[n-3].
 * https://oeis.org/A087775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1, a(2)=2, a(3)=2; For n≥4:
 *            a(n)=3*a[abs(a[n-2])]-3*a[n-abs(a[n-2])]+a[n-3]. Closed‑form: if
 *            n mod 6 is 1 or 4 → a(n)=1 if n mod 6 is 2 or 5 → a(n)=2 if n
 *            mod 6 is 0 or 3 → a(n)=-(n-5) (i.e. 5−n)
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned r = n % 6;
    if (r == 1 || r == 4)
        return (seq_t)1;
    if (r == 2 || r == 5)
        return (seq_t)2;
    /* r == 0 or r == 3 */
    return -((seq_t)n - (seq_t)5);
}
