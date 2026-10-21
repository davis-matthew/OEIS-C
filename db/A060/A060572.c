/*
 * A060572  Tower of Hanoi: the optimal way to move an even number of disks
 *          from peg 0 to peg 2 or an odd number from peg 0 to peg 1 is on
 *          move n to move disk A001511 from peg A060571 to peg A060572
 *          (here).
 * https://oeis.org/A060572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = destination peg on move n in the optimal 3‑peg Tower of
 *            Hanoi when moving an odd number of disks from peg 0 to peg 1.
 *            Let t = v₂(n) (number of trailing zero bits in n). Disk k = t+1
 *            is moved on move n. Direction of disk k: +1 (mod 3) if k is odd,
 *            –1 ≡ 2 (mod 3) if k is even. Destination =...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 */
    unsigned t = 0;
    idx_t tmp = n;
    /* find v₂(n) */
    while ((tmp & 1ULL) == 0) {
        t++;
        tmp >>= 1;
    }
    unsigned k = t + 1;                /* disk index */
    unsigned delta = (k & 1U) ? 1U : 2U;  /* 1 for odd disk, 2 for even disk (−1 mod 3) */

    idx_t j;
    /* shift by 64 is undefined, handle separately */
    if (k == 64) {
        j = 0;
    } else {
        j = n >> k;
    }

    unsigned rem = (unsigned)((j + 1ULL) % 3ULL);
    unsigned dest = (delta * rem) % 3U;
    return (seq_t)dest;
}
