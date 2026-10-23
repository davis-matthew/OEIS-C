/*
 * A028424  Clog sequence in base 3. Right to left concatenation of
 *          n,int(log_3(n)),int(log_3(int(log_3(n)))),... in base 3.
 * https://oeis.org/A028424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = right‑to‑left concatenation in base 3 of the list (n,
 *            ⌊log₃(n)⌋, ⌊log₃⌊log₃(n)⌋⌋, …, 0) (OEIS A028424)
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_log3_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log3_u64.h"

/* Number of base‑3 digits of x (x ≥ 0).  */
static inline unsigned digits_of(idx_t x)
{
    if (x == 0)
        return 1;
    unsigned d = 0;
    while (x) {
        x /= 3;
        ++d;
    }
    return d;
}

/* 3ᵉ (e ≥ 0).  */
static inline seq_t pow3(unsigned e)
{
    seq_t r = 1;
    for (unsigned i = 0; i < e; ++i)
        r *= 3;
    return r;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Build the chain: n, ⌊log₃(n)⌋, ⌊log₃⌊log₃(n)⌋⌋, … */
    idx_t chain[32];
    size_t len = 0;

    idx_t cur = n;
    chain[len++] = cur;          /* the first item is n itself */

    while (cur > 1) {
        cur = floor_log3_u64(cur);
        chain[len++] = cur;
    }

    /* Concatenate from most significant (chain[len‑1]) to least
       significant (chain[0]) in base 3.  */
    seq_t result = 0;
    for (long long i = (long long)len - 1; i >= 0; --i) {
        idx_t part = chain[i];
        unsigned d   = digits_of(part);
        result = result * pow3(d) + (seq_t)part;
    }

    return result;
}
