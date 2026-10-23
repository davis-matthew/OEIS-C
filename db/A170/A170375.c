/*
 * A170375  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of reduced words of length n in the Coxeter group on 30 generators
    with relations (S_i)^2 = (S_i S_j)^43 = I.
    The sequence satisfies the linear recurrence:

        a(0)   = 1
        a(n)   = 2 + 28 * Σ_{k=0}^{n-1} a(k)                      for 1 ≤ n ≤ 42
        a(43)  = 28 * Σ_{k=0}^{42} a(k) – 405
        a(n)   = 28 * Σ_{i=1}^{42} a(n-i) – 406 * a(n-43)        for n ≥ 44
    (Here Σ denotes a sum of the preceding terms.)

    The recurrence originates from the rational generating function
    G(t) = (t^43 + 2 t^42 + … + 2 t + 1)/(1 – 28 t – … – 28 t^42 + 406 t^43).
*/

seq_t a(idx_t n)
{
    /* The maximal argument expected in the tests is about 10 000.  */
    const idx_t MAX_N = 10000;
    /* The array is large enough for any required n and is stack‑allocated.
       This works both for plain integer types (uint8_t … uint128_t) and
       for the arbitrary‑precision integer class used in the C++ build. */
    if (n > MAX_N) {
        /* Fallback for extremely large n (should not happen in tests). */
        /* Allocate a dynamic array and compute on the fly. */
        seq_t *buf = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!buf) return (seq_t)0;       /* memory error → return 0 */

        buf[0] = (seq_t)1;
        seq_t prefix_sum = (seq_t)1;

        /* Terms 1 … 42 */
        for (idx_t i = 1; i <= n && i <= 42; ++i) {
            buf[i] = (seq_t)2 + (seq_t)28 * prefix_sum;
            prefix_sum += buf[i];
        }

        if (n >= 43) {
            /* a(43) */
            buf[43] = (seq_t)28 * prefix_sum - (seq_t)405;
            if (n == 43) {
                seq_t res = buf[43];
                free(buf);
                return res;
            }
            /* Further terms */
            for (idx_t i = 44; i <= n; ++i) {
                seq_t sum_42 = (seq_t)0;
                for (idx_t j = 1; j <= 42; ++j)
                    sum_42 += buf[i - j];
                buf[i] = (seq_t)28 * sum_42 - (seq_t)406 * buf[i - 43];
            }
        }

        seq_t ans = buf[n];
        free(buf);
        return ans;
    }

    /* Small enough n – use a fixed size stack array of 10001 elements. */
    seq_t a[10001];
    a[0] = (seq_t)1;
    seq_t prefix_sum = (seq_t)1;

    for (idx_t i = 1; i <= n && i <= 42; ++i) {
        a[i] = (seq_t)2 + (seq_t)28 * prefix_sum;
        prefix_sum += a[i];
    }

    if (n >= 43) {
        a[43] = (seq_t)28 * prefix_sum - (seq_t)405;
        if (n == 43) return a[43];
        for (idx_t i = 44; i <= n; ++i) {
            seq_t sum_42 = (seq_t)0;
            for (idx_t j = 1; j <= 42; ++j)
                sum_42 += a[i - j];
            a[i] = (seq_t)28 * sum_42 - (seq_t)406 * a[i - 43];
        }
    }

    return a[n];
}
