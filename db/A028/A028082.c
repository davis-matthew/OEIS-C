/*
 * A028082  Expansion of 1/((1-3x)(1-6x)(1-8x)(1-12x)).
 * https://oeis.org/A028082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*12^(n+3)-27*8^(n+3)+30*6^(n+3)-8*3^(n+3))/1080
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence also satisfies the recurrence:        */
/*   a(0)=1, a(1)=29, a(2)=547, a(3)=8561                 */
/*   a(n) = 29*a(n-1) - 294*a(n-2) + 1224*a(n-3) - 1728*a(n-4)  for n≥4 */

seq_t a(idx_t n)
{
    /* Handle the four initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)547;
    if (n == 3) return (seq_t)8561;

    /* Compute successive terms using the 4‑term linear recurrence */
    seq_t a0 = (seq_t)1;    /* a(n-4)   */
    seq_t a1 = (seq_t)29;   /* a(n-3)   */
    seq_t a2 = (seq_t)547;  /* a(n-2)   */
    seq_t a3 = (seq_t)8561; /* a(n-1)   */
    seq_t an = a3;         /* holds a(n) during the loop */

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)29 * a3
           - (seq_t)294 * a2
           + (seq_t)1224 * a1
           - (seq_t)1728 * a0;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
