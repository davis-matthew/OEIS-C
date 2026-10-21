/*
 * A055119  Base-9 complement of n (write n in base 9, then replace each digit
 *          with its base-9 negative).
 * https://oeis.org/A055119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = base‑9 complement of n
 * Range:     u8 n <= 80, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The complement is obtained by replacing each base‑9 digit d with (9‑d) mod 9. */
seq_t a(idx_t n)
{
    seq_t res   = 0;      /* accumulated result */
    seq_t power = 1;      /* current power of 9 (9^i) */
    idx_t rem   = n;      /* working copy of n */

    while (rem > 0) {
        idx_t d = rem % 9;                    /* current base‑9 digit */
        seq_t comp = (d == 0) ? 0 : (seq_t)(9 - d); /* complement digit */
        res += comp * power;                  /* add contribution at this position */
        rem /= 9;                             /* move to next higher digit */
        power *= 9;                            /* next power of 9 */
    }

    return res;
}
