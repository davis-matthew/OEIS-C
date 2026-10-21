/*
 * A064376  Numbers n such that sigma_3(n) > phi(n)^4.
 * https://oeis.org/A064376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: collect all integers n such that sigma_3(n) > phi(n)^4.
   The function returns the n-th such integer (a(1)=2).  Results are memoized
   so that calls can arrive in any order.  The full sequence is finite,
   so once all terms are found no further work is done. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* maximum n to search (more than enough for the known finite set) */
#if UINTPTR_MAX > 0xFFFFFFFF
#define MAX_SCAN 10000000ULL
#else
#define MAX_SCAN 2000000ULL
#endif

/* static array to store the solutions – the sequence has far fewer
   than 128 terms, so 4096 slots are plenty even in a 128‑bit build. */
static seq_t solutions[4096];
static size_t sol_len   = 0;          /* number of terms stored   */
static idx_t checked_until = 0;      /* first n not yet examined */

/* fast exponentiation for seq_t with a small integer exponent */
static inline seq_t pow_seq_small(seq_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* generate all terms until we have at least target terms or we run out
   of the search range.  The function is idempotent – it continues
   from the last value evaluated. */
static void generate_to(size_t target)
{
    if (sol_len >= target) return;

    if (checked_until < (idx_t)2) checked_until = (idx_t)2;

    for (idx_t n = checked_until; n <= MAX_SCAN; ++n) {
        uint64_t val = (uint64_t)n;
        uint64_t primes[16];
        uint64_t exps[16];
        size_t cnt = 0;
        factor_u64(val, primes, exps, &cnt);

        seq_t sigma3 = (seq_t)1;   /* sigma_3(n)   */
        seq_t phi    = (seq_t)1;   /* phi(n)      */

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p   = primes[i];
            uint64_t e   = exps[i];
            seq_t   p_seq = (seq_t)p;

            /* phi part : p^(e-1) * (p-1) */
            seq_t p_pow_e_minus1 = (e > 1) ? pow_seq_small(p_seq, e-1) : (seq_t)1;
            seq_t phi_part = p_pow_e_minus1 * (seq_t)(p - 1);
            phi *= phi_part;

            /* sigma_3 part : (p^{3(e+1)} - 1)/(p^3 - 1) */
            seq_t num = pow_seq_small(p_seq, 3*(e+1));
            num -= (seq_t)1;
            seq_t den = pow_seq_small(p_seq, 3);
            den -= (seq_t)1;
            seq_t sigma_part = num / den;

            sigma3 *= sigma_part;
        }

        /* phi^4 */
        seq_t phi4 = phi * phi;
        phi4       = phi4 * phi4;   /* (phi^2)^2 = phi^4 */

        if (sigma3 > phi4) {
            /* store as seq_t */
            solutions[sol_len++] = (seq_t)n;
            if (sol_len >= target) {
                checked_until = n + (idx_t)1;
                return;
            }
        }
    }

    /* nothing more to find in the search range */
    checked_until = MAX_SCAN + (idx_t)1;
}

/* public interface: return the n-th term of the sequence, a(1)=2. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */
    generate_to((size_t)n);
    return (n <= (idx_t)sol_len) ? solutions[(size_t)(n-1)] : (seq_t)0;
}
