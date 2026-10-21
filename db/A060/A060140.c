/*
 * A060140  Ordered set S defined by these rules: 0 and 1 are in S and if x is
 *          a nonzero number in S, then 3x and 9x+1 are in S.
 * https://oeis.org/A060140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ordered set S (OEIS A060140) -------------------------------------------------
   Sequence:  a(0) = 0, a(1) = 1, a(2) = 3, a(3) = 9, a(4) = 10, a(5) = 27, …
   Definition: 0, 1 ∈ S and if x ∈ S, then 3x ∈ S and (9x+1) ∈ S.
   These are exactly the integers whose ternary representation consists
   only of digits 0 and 1 with no two consecutive 1s (Fibternary numbers).
   The algorithm below finds the n‑th term by combinatorial enumeration of
   those ternary strings, which runs in O(L) with L ≈ log_φ(n).
*/

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) is the special zero term */
    if (n == 0) return 0;

    /* --------------------------------------------------------------
       Compute Fibonacci numbers up to the required index.
       We only need them as uint64_t (idx_t), which is large enough
       for the index range used in the tests.
       -------------------------------------------------------------- */
    static idx_t fib[70];
    static bool ready = false;
    if (!ready) {
        fib[0] = 0;
        fib[1] = 1;
        for (size_t i = 2; i < 70; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        ready = true;
    }

    /* --------------------------------------------------------------
       Find the minimal ternary length L such that
       F_{L+2} - 1 ≥ n  (n counts the positive terms only, starting from a(1)=1)
       -------------------------------------------------------------- */
    size_t L = 1;
    while (fib[L + 2] - 1 < n)
        ++L;

    /* --------------------------------------------------------------
       Compute the 0‑based offset inside the block of length‑L numbers.
       offset0 = n - F_{L+1}
       -------------------------------------------------------------- */
    idx_t offset_idx = n - fib[L + 1];
    seq_t offset = (seq_t)offset_idx;       /* fits when the term fits in seq_t */

    /* --------------------------------------------------------------
       DP to count completions of a ternary suffix.
       dp[r][p] = number of length‑r strings starting with previous
       digit equal to p (0 or 1) and containing no consecutive 1s.
       Only r up to L-1 is needed.
       -------------------------------------------------------------- */
    static seq_t dp[128][2];
    dp[0][0] = dp[0][1] = 1;
    for (size_t i = 1; i <= L - 1; ++i) {
        dp[i][0] = dp[i - 1][0] + dp[i - 1][1];
        dp[i][1] = dp[i - 1][0];
    }

    /* --------------------------------------------------------------
       Build the ternary digits from the most significant side.
       Start with the leading 1; the next digit (pos 1) is forced 0.
       -------------------------------------------------------------- */
    seq_t result = 1;
    int prev = 1;              /* previous digit, starts with the leading 1 */

    for (size_t pos = 1; pos < L; ++pos) {
        size_t rem = (L - 1) - pos;          /* remaining positions after this */

        if (prev == 1) {                     /* cannot place 1, must place 0 */
            result = result * 3;
            prev   = 0;
        } else {
            seq_t count0 = dp[rem][0];       /* completions if we put 0 here */
            if (offset < count0) {
                result = result * 3;
                prev   = 0;
            } else {
                offset -= count0;
                result = result * 3 + 1;
                prev   = 1;
            }
        }
    }

    return result;
}
