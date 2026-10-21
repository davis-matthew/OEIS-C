/*
 * A098476  Unicode codes for the lunation runes, used in certain medieval
 *          Scandinavian perpetual calendar staves as golden numbers 1-19.
 * https://oeis.org/A098476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = L[(n-1) mod 19] where L is the list of rune code points
 *            for the 19 lunation runes. This repeats every 19 entries
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t lunation_runes[19] =
{
    5792,  /* a(1) */
    5794,  /* a(2) */
    5798,  /* a(3) */
    5805,  /* a(4) */
    5809,  /* a(5) */
    5812,  /* a(6) */
    5820,  /* a(7) */
    5823,  /* a(8) */
    5825,  /* a(9) */
    5830,  /* a(10) */
    5835,  /* a(11) */
    5839,  /* a(12) */
    5842,  /* a(13) */
    5850,  /* a(14) */
    5848,  /* a(15) */
    5862,  /* a(16) */
    5870,  /* a(17) */
    5871,  /* a(18) */
    5872   /* a(19) */
};

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 19: for any n >= 1,
       the rune code is determined by (n-1) mod 19. */
    idx_t idx = (n - 1) % 19;
    return lunation_runes[idx];
}
