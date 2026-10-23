/*
 * A105672  a(1)=1, then bracketing n with powers of 3 as f(t)=3^t for f(t) <
 *          n <= f(t+1), a(n) = f(t+1) - a(n-f(t)).
 * https://oeis.org/A105672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1 let f(t)=3^t with f(t) < n <= f(t+1); then
 *            a(n) = f(t+1) - a(n - f(t))
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Local static tables for memoisation and powers of 3. */
    static seq_t dp[10001];          /* dp[1] .. dp[10000] computed values */
    static idx_t current = 1;        /* highest index computed so far */

    static seq_t powers[20];
    static idx_t power_len = 0;
    static int initialized = 0;

    /* Initialise tables on first call. */
    if (!initialized) {
        powers[0] = (seq_t)1;
        idx_t i = 0;
        while (1) {
            seq_t next = powers[i] * (seq_t)3;
            powers[i + 1] = next;
            ++i;
            if (next > (seq_t)100000) break;   /* enough for n up to 10000 */
        }
        power_len = i + 1;          /* powers[0..power_len] valid */
        dp[0] = (seq_t)0;          /* unused */
        dp[1] = (seq_t)1;          /* a(1) */
        initialized = 1;
    }

    if (n < 1) return (seq_t)0;     /* value for n<1 is undefined, return 0 */

    if (n > 10000) return (seq_t)0; /* out‑of‑range guard */

    /* Compute values up to n if needed, in any call order. */
    if (n > current) {
        for (idx_t idx = current + 1; idx <= n; ++idx) {
            if (idx == 1) {                /* base case */
                dp[1] = (seq_t)1;
                continue;
            }

            /* Find t with powers[t] < idx <= powers[t+1] */
            idx_t t = 0;
            while (idx > powers[t + 1]) ++t;

            seq_t ft     = powers[t];
            seq_t fnext  = powers[t + 1];

            dp[idx] = fnext - dp[idx - (idx_t)ft];
        }
        current = n;
    }

    return dp[n];
}
