/*
 * A220022  The period with which the powers of n repeat mod 100.
 * https://oeis.org/A220022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the minimal positive period k of the sequence n^i (mod
 *            100). That is, the smallest k such that the sequence of residues
 *            n^0, n^1, n^2, … eventually repeats with period k
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A220022 for index n */
seq_t a(idx_t n)
{
    /* Residue of n modulo 100 -- deterministic because of periodicity. */
    const unsigned int MOD = 100;
    unsigned int n_mod = (unsigned int)(n % MOD);

    /* visited[i] = first position at which residue i was seen,
       or -1 if not seen yet.  The maximum number of residues is 100. */
    int visited[MOD];
    for (int i = 0; i < MOD; ++i) visited[i] = -1;

    int idx = 0;            /* current step number (exponent)          */
    int cur = 1;            /* current residue 1 = n^0 mod 100         */

    while (1) {
        if (visited[cur] != -1) {
            /* Cycle length: difference between current position and first time we saw this residue. */
            int cycle = idx - visited[cur];
            return (seq_t)cycle;
        }
        visited[cur] = idx;          /* record first visit */
        ++idx;
        cur = (int)(((unsigned int)cur * n_mod) % MOD);
    }
}
