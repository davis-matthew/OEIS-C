/*
 * A200435  Number of -n..n arrays x(0..7) of 8 elements with zero sum and no
 *          two or three adjacent elements summing to zero.
 * https://oeis.org/A200435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200435
   Recurrence: a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3)
                   - 70*a(n-4) + 56*a(n-5) - 28*a(n-6)
                   +  8*a(n-7) - a(n-8)    for n > 8
   Initial values: a(1)=0, a(2)=1928, a(3)=50592, a(4)=473034,
                   a(5)=2591502, a(6)=10217182, a(7)=32233938, a(8)=86581308
*/

seq_t a(idx_t n)
{
    /* Static initial values (index 0 unused) */
    static const seq_t seeds[9] = {
        (seq_t)0,          /* unused index 0 */
        (seq_t)0,          /* a(1) */
        (seq_t)1928,       /* a(2) */
        (seq_t)50592,      /* a(3) */
        (seq_t)473034,     /* a(4) */
        (seq_t)2591502,    /* a(5) */
        (seq_t)10217182,   /* a(6) */
        (seq_t)32233938,   /* a(7) */
        (seq_t)86581308    /* a(8) */
    };

    /* Return cached value for n <= 8 */
    if (n <= 8)
        return seeds[(idx_t)n];

    /* Initial 8 values for iterative recurrence */
    seq_t a1 = seeds[1];
    seq_t a2 = seeds[2];
    seq_t a3 = seeds[3];
    seq_t a4 = seeds[4];
    seq_t a5 = seeds[5];
    seq_t a6 = seeds[6];
    seq_t a7 = seeds[7];
    seq_t a8 = seeds[8];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next;
        /* Use 128‑bit intermediate when seq_t is 64‑bit to avoid overflow.
           For other seq_t types (e.g. arbitrary‑precision class) regular
           operations are safe. */
        if (sizeof(seq_t) == 8) {
            signed __int128 t;
            t = (signed __int128)a8 * 8;
            t -= (signed __int128)a7 * 28;
            t += (signed __int128)a6 * 56;
            t -= (signed __int128)a5 * 70;
            t += (signed __int128)a4 * 56;
            t -= (signed __int128)a3 * 28;
            t += (signed __int128)a2 * 8;
            t -= (signed __int128)a1;
            next = (seq_t)t;
        } else {
            next = (seq_t)(8 * a8  - 28 * a7 + 56 * a6
                           - 70 * a5 + 56 * a4 - 28 * a3
                           +  8 * a2 - a1);
        }

        /* Shift the window of the last eight terms */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = next;
    }
    return a8;
}
