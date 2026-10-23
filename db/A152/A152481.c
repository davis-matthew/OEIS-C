/*
 * A152481  Degrees of irreducible representations of SL(2,11).
 * https://oeis.org/A152481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th degree (in ascending order) of the irreducible
 *            complex representations of SL(2,11^k), where k is chosen so that
 *            the cumulative number of degrees up to q=11^k covers n. The
 *            degrees for a given power q are: 1 (q-1)/2 – twice (q+1)/2 –
 *            twice q-1 – (q-1)/2 times q – once q+1 – (q-3)/...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the power q=11^k such that the cumulative count
       of terms f(q)=q+4 reaches or exceeds n. */
    uint64_t q = 11;          /* current power of 11 */
    uint64_t cum = 0;         /* cumulative terms before current q */

    while (cum + q + 4 < n)   /* if even after adding count of current q, we still are below n, go to next q */
    {
        cum += q + 4;         /* add the full list of current q */
        q *= 11;              /* move to next power of 11 */
    }

    /* m is the position of the desired term within the list for this q (1‑based). */
    idx_t m = n - cum;

    /* Precompute useful integer values in normal C integers.
       For all tested n (<2^64), q fits in uint64_t.               */
    uint64_t a_val = (q - 1) / 2;   /* (q-1)/2  – appears twice   */
    uint64_t b_val = (q + 1) / 2;   /* (q+1)/2  – appears twice   */

    /* Determine which block m falls into.           */
    if (m == 1)                         /* the trivial representation */
        return (seq_t)1;

    if (m <= 3)                         /* two copies of (q-1)/2   */
        return (seq_t)a_val;

    if (m <= 5)                         /* two copies of (q+1)/2   */
        return (seq_t)b_val;

    if (m <= 5 + a_val)                 /* (q-1) repeated a_val times  */
        return (seq_t)(q - 1);

    if (m == 6 + a_val)                 /* single copy of q          */
        return (seq_t)q;

    /* Remaining positions: (q+1) repeated (q-3)/2 times. */
    return (seq_t)(q + 1);
}
