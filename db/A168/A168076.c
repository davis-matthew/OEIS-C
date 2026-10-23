/*
 * A168076  Expansion of 1 - 3*(1-x-sqrt(1-2*x-3*x^2))/2.
 * https://oeis.org/A168076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 24, i64 n <= 45, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168076
   a(0)=1, a(1)=0, a(2)=-3;
   for n≥3: n*a(n)+(-2n+3)*a(n-1)+3(-n+3)*a(n-2)=0  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_nm2 = (seq_t)1;      /* a(0) */
    seq_t a_nm1 = (seq_t)0;      /* a(1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        if (i == 2) {
            a_n = (seq_t)-3;
        } else {
            /* Use 128‑bit intermediate when seq_t is a fixed‑size integer. */
            if (sizeof(seq_t) <= sizeof(long long)) {
                signed __int128 t1 = (signed __int128)a_nm1 * (signed __int128)(2*i-3);
                signed __int128 t2 = (signed __int128)a_nm2 * (signed __int128)(3*(i-3));
                signed __int128 sum = t1 + t2;
                a_n = (seq_t)(sum / (seq_t)i);
            } else {
                /* seq_t is an arbitrary‑precision class: use its operators. */
                seq_t t1 = a_nm1 * (seq_t)(2*i-3);
                seq_t t2 = a_nm2 * (seq_t)(3*(i-3));
                a_n = (t1 + t2) / (seq_t)i;
            }
        }
        a_nm2 = a_nm1;   /* shift a(n-1) and a(n-2) */
        a_nm1 = a_n;
    }
    return a_n;
}
