/*
 * A043283  Maximal run length in base-9 representation of n.
 * https://oeis.org/A043283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum length of a run of equal consecutive digits in
 *            the base‑9 representation of n
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    int prev = -1;          /* previous digit, -1 guarantees a reset on first iteration */
    seq_t curr = 0;         /* current run length */
    seq_t maxRun = 0;       /* maximum run length seen so far */

    /* Handle the trivial case n == 0 (not part of the OEIS offset but harmless) */
    if (t == 0)
        return 0;

    while (t > 0) {
        int d = (int)(t % 9);  /* digit in base‑9 (value 0–8) */
        t /= 9;
        if (d == prev)
            curr++;                     /* extend current run */
        else {
            curr = 1;                  /* start new run */
            prev = d;
        }
        if (curr > maxRun)
            maxRun = curr;
    }
    return maxRun;
}
