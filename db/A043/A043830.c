/*
 * A043830  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 4.
 * https://oeis.org/A043830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the nth integer whose number of runs in its base‑4
    representation is congruent to 2 (mod 4).  This is OEIS sequence
    A043830.  The implementation simply scans the positive integers
    and counts those with the required property, stopping when the
    nth such integer has been found.  */
    
/*  Helper: count runs of equal consecutive digits in the base‑4
    representation of a number.  */
static unsigned run_count_base4(seq_t x)
{
    unsigned runs = 0;
    if (x == 0)
        return 0;                  /* by convention, zero has no runs */

    seq_t last = x % 4;             /* least significant digit */
    x     /= 4;
    runs = 1;                       /* first run */

    while (x != 0) {
        seq_t d = x % 4;
        if (d != last) {
            ++runs;
            last = d;
        }
        x /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    idx_t found = 0;                /* number of qualifying integers seen */
    seq_t k     = 1;                /* candidate integer */

    while (found < n) {
        if (run_count_base4(k) % 4 == 2) {   /* runs ≡ 2 (mod 4) */
            ++found;
            if (found == n)
                return k;
        }
        ++k;                       /* next integer */
    }
    /* Control should never reach here for n >= 1 */
    return 0;
}
