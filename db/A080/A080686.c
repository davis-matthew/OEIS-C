/*
 * A080686  Number of 19-smooth numbers <= n.
 * https://oeis.org/A080686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of 19‑smooth numbers ≤ n * (numbers all of whose
 *            prime factors are ≤ 19)
 * Range:     u8 n <= 648, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9382 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void smooth_count(seq_t cur, seq_t lim, idx_t start, seq_t *cnt,
                         const seq_t primes[8]) {
    if (cur > lim)
        return;
    ++(*cnt);
    for (idx_t i = start; i < 8; ++i) {
        if (cur > lim / primes[i])
            continue;
        smooth_count(cur * primes[i], lim, i, cnt, primes);
    }
}

seq_t a(idx_t n) {
    const seq_t primes[8] = {2, 3, 5, 7, 11, 13, 17, 19};
    seq_t cnt = 0;
    smooth_count((seq_t)1, (seq_t)n, 0, &cnt, primes);
    return cnt;
}
