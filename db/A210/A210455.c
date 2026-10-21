/*
 * A210455  Characteristic function of pseudoperfect (or semiperfect) numbers.
 * https://oeis.org/A210455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210455: 1 if n is a pseudoperfect (semiperfect) number, 0 otherwise.
   The function factors n, enumerates all divisors, then performs a
   standard subset‑sum DP to decide whether some subset of proper
   divisors sums to n.  Complexity is O(n·d(n)), fast enough for
   n up to 10⁴. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursively generate all divisors of n from its prime factorization. */
static void gen_divisors_rec(const uint64_t *pr, const uint64_t *ex,
                             size_t cnt, size_t idx,
                             idx_t cur, idx_t *buf, size_t *len)
{
    if (idx == cnt) {
        if (*len < 512) {
            buf[(*len)++] = cur;
        }
        return;
    }
    uint64_t p = pr[idx];
    idx_t c = cur;
    for (uint64_t k = 0; k <= ex[idx]; ++k) {
        gen_divisors_rec(pr, ex, cnt, idx + 1, c, buf, len);
        c *= p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* 1 is not pseudoperfect */
        return (seq_t)0;

    /* factor n into primes and exponents */
    uint64_t primes[16], exps[16];
    size_t pfcnt = 0;
    factor_u64(n, primes, exps, &pfcnt);

    /* generate all divisors, including n itself */
    idx_t divisors[512];
    size_t dcnt = 0;
    gen_divisors_rec(primes, exps, pfcnt, 0, (idx_t)1, divisors, &dcnt);

    /* subset‑sum DP: can we reach sum n using proper divisors? */
    int N = (int)n;
    seq_t *reach = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!reach) return (seq_t)0;          /* allocation failure → 0 */

    reach[0] = 1;                         /* sum 0 is always achievable */

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divisors[i];
        if (d == n || d == 0) continue;   /* ignore n itself and zero */
        int ds = (int)d;
        if (ds > N) continue;
        for (int s = N; s >= ds; --s) {
            if (reach[s - ds]) reach[s] = 1;
        }
    }

    seq_t ans = reach[N] ? 1 : 0;
    free(reach);
    return ans;
}
