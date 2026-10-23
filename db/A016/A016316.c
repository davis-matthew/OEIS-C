/*
 * A016316  Expansion of 1/((1-2x)*(1-8x)*(1-9x)).
 * https://oeis.org/A016316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=19, a(2)=255,
 * a(n)=19*a(n-1)-106*a(n-2)+144*a(n-3)  (n≥3)              */
/* Note: all calculations are carried out in seq_t, which may be
 * a fixed-width unsigned integer or an arbitrary-precision type.   */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)19;
    if (n == (idx_t)2) return (seq_t)255;

    const seq_t C19  = (seq_t)19;
    const seq_t C106 = (seq_t)106;
    const seq_t C144 = (seq_t)144;

    seq_t a_m3 = (seq_t)1;   /* a(n-3)   */
    seq_t a_m2 = (seq_t)19;  /* a(n-2)   */
    seq_t a_m1 = (seq_t)255; /* a(n-1)   */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t temp = C19 * a_m1 + C144 * a_m3;
        seq_t next = temp - C106 * a_m2;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = next;
    }
    return a_m1;
}
