/*
 * A028088  Expansion of 1/((1-3x)(1-6x)(1-11x)(1-12x)).
 * https://oeis.org/A028088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*12^(n+3)-27*11^(n+3)+12*6^(n+3)-5*3^(n+3))/1080
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 32*a(n-1) - 357*a(n-2) + 1602*a(n-3) - 2376*a(n-4) */
/* with a(0)=1, a(1)=32, a(2)=667, a(3)=11522 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 0) return 1;
    if (n == 1) return 32;
    if (n == 2) return 667;
    if (n == 3) return 11522;

    /* Current values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1;      /* a(0) */
    seq_t a1 = 32;     /* a(1) */
    seq_t a2 = 667;    /* a(2) */
    seq_t a3 = 11522;  /* a(3) */

    /* Compute successive terms using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next;

#ifdef __cplusplus
        /* In C++ the arithmetic operators are overloaded for arbitrary‑precision types */
        next = 32 * a3 - 357 * a2 + 1602 * a1 - 2376 * a0;
#else
        /* In C use a 128‑bit intermediate to avoid overflow for 64‑bit builds */
        __int128 t = (__int128)32 * (__int128)a3
                   - (__int128)357 * (__int128)a2
                   + (__int128)1602 * (__int128)a1
                   - (__int128)2376 * (__int128)a0;
        next = (seq_t)t;
#endif

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
