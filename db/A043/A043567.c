/*
 * A043567  Number of runs in base-15 representation of n.
 * https://oeis.org/A043567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base‑15 representation of n
 * Range:     u8 n <= 8249, u16 n <= 8249, u32 n <= 8249, u64 n <= 8249,
 *            u128 n <= 8249, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 0 has the single digit "0" → 1 run */
    if (n == 0) return 1;

    seq_t runs  = 0;         /* count of runs found so far      */
    seq_t prev  = 15;        /* sentinel value (outside 0‑14)    */
    idx_t  temp = n;         /* copy of the input for manipulation */

    /* Scan the digits in reverse order (least significant first).
       The number of runs does not depend on the direction of the scan,
       because a run is a maximal block of identical digits. */
    while (temp > 0) {
        idx_t digit = temp % 15;
        seq_t sd    = (seq_t)digit;
        if (sd != prev) {
            ++runs;        /* a new run starts with this digit */
            prev = sd;
        }
        temp /= 15;
    }
    return runs;
}
