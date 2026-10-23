/*
 * A122520  Expansion of x*(1+2*x+3*x^2+4*x^3+4*x^4)/(1+x+x^2+x^3-x^5).
 * https://oeis.org/A122520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 69, i32 n <= 137, i64 n <= 271,
 *            i128 n <= 543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) - a(n-2) - a(n-3) + a(n-5)  for n >= 6,
   with initial values a(1)=a(2)=a(3)=a(4)=a(5)=1.  */
seq_t a(idx_t n)
{
    if (n <= 5)          /* offsets 1‑5 are all 1 */
        return (seq_t)1;

    /* Buffer holds the last five computed terms:
         buf[0] = a(n-5), buf[1] = a(n-4), ..., buf[4] = a(n-1)    */
    seq_t buf[5];
    for (idx_t i = 0; i < 5; ++i)
        buf[i] = (seq_t)1;          /* initialise a(1)…a(5) */

    seq_t new_val;
    for (idx_t i = 6; i <= n; ++i) {
        new_val = -buf[4] - buf[3] - buf[2] + buf[0];  /* a(i) = -a(i‑1)-a(i‑2)-a(i‑3)+a(i‑5) */
        /* shift the buffer left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = new_val;
    }
    return buf[4];                      /* a(n) */
}
