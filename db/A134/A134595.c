/*
 * A134595  a(n) is the smallest number such that, for any m, the number
 *          formed from the first m digits of a(n) is congruent to n mod m;
 *          but no digit can be appended to maintain the condition.
 * https://oeis.org/A134595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A134595(a(n)): the smallest 10‑digit integer with the property
 * that for all m = 1 .. 10, the number formed by its first m digits is
 * congruent to n modulo m, and that the number cannot be extended
 * without breaking the property.  The implementation performs a
 * depth‑first search over the at most two possible digits at each
 * position, pruning prefixes that cannot lead to a complete solution.
 * It always chooses the lexicographically smallest solution, which
 * yields the minimal integer.
 */
#include <stdbool.h>

/* Recursive helper: try to extend the current prefix (first m‑1 digits)
 * to a valid 10‑digit number.  `digits` stores the chosen digits.
 */
static bool dfs(int pos, seq_t prefix, seq_t sn, int *digits)
{
    /* pos ranges from 1 to 11; when pos == 11 we have already chosen
     * 10 digits (pos-1 == 10).  Validate the no‑extension condition. */
    if (pos == 11) {
        /* no digit d (0‑9) can be appended to preserve the congruence
         * modulo 11. */
        for (int d = 0; d <= 9; ++d) {
            seq_t val = prefix * 10 + d;
            if ((val % 11) == (sn % 11))
                return false;          /* extension possible; reject */
        }
        return true;                   /* valid completion found */
    }

    /* Compute the required digit modulo pos.  We use idx_t for the
     * small arithmetic to avoid unnecessary large‑integer work. */
    const idx_t m = pos;                 /* modulus for this position   */
    const idx_t nmod = (idx_t)(sn % m);       /* n mod m                     */
    const idx_t tenpmod = (idx_t)((prefix * 10) % m); /* (10·prefix) mod m   */
    const idx_t rhs = (nmod + m - tenpmod) % m;      /* needed remainder  */

    /* Try all digits of the form rhs + k·m that are ≤ 9. */
    for (idx_t k = 0;; ++k) {
        const idx_t d = rhs + k * m;
        if (d > 9) break;               /* digits exhausted              */
        if (pos == 1 && d == 0) continue; /* leading zero not allowed     */
        digits[pos - 1] = (int)d;
        const seq_t newpref = prefix * 10 + d;
        if (dfs(pos + 1, newpref, sn, digits))
            return true;                /* found a full solution        */
    }
    return false; /* no digit leads to completion */
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t once. */
    const seq_t sn = (seq_t)n;

    /* Array to hold the 10 digits. */
    int digits[10] = {0};

    /* Build the number.  The recursion guarantees a solution exists. */
    bool ok = dfs(1, 0, sn, digits);

    /* The test harness guarantees ok == true for valid inputs; we
     * nevertheless handle it defensively. */
    if (!ok) {
        /* In the unlikely event of an error, return 0. */
        return 0;
    }

    /* Assemble the result from the digit array. */
    seq_t result = 0;
    for (int i = 0; i < 10; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
