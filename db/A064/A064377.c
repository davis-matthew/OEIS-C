/*
 * A064377  Numbers n such that sigma_4(n) > phi(n)^5.
 * https://oeis.org/A064377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers n such that sigma_4(n) > phi(n)^5. * The function
 *            a(idx_t n) returns the nth term of OEIS A064377 * (sorted
 *            ascending). Terms are generated on demand using * phi,
 *            factor_u64, seq_pow and divide/multiply on seq_t values. * After
 *            the finite list of solutions the function returns 0
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 111, u128 n <= 111,
 *            big any n
 * Helpers:   factor_u64, grow_seq, phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"
#include "../utils/grow_seq.h"
#include "../utils/seq_pow.h"

static size_t seq_len = 0;          /* number of terms found so far   */
static seq_t *seq_arr = NULL;       /* array holding the terms        */
static size_t seq_cap = 0;          /* current capacity of seq_arr    */
static uint64_t last_checked = 1;   /* next n to evaluate             */

static void ensure_terms(idx_t target)
{
    /* Generate terms until we have at least `target` terms or
       until we exceed a safe upper bound (the sequence is
       known to be finite and ends below 50). */
    const uint64_t UPPER_LIMIT = 1000000ULL;
    if (seq_len >= (size_t)target) return;

    while (seq_len < (size_t)target && last_checked < UPPER_LIMIT)
    {
        ++last_checked;
        /* Compute phi(last_checked) */
        seq_t phi_val = phi(last_checked);

        /* Compute sigma_4(last_checked) */
        uint64_t n = last_checked;
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(n, primes, exps, &cnt);

        seq_t sigma4 = 1;
        for (size_t i = 0; i < cnt; ++i)
        {
            seq_t p   = primes[i];
            uint64_t e = exps[i];

            seq_t num = seq_pow(p, 4 * (e + 1)) - 1;
            seq_t den = seq_pow(p, 4) - 1;
            seq_t term = num / den;
            sigma4 *= term;
        }

        seq_t phi5 = seq_pow(phi_val, 5);
        if (sigma4 > phi5)
        {
            if (seq_len == seq_cap)
                grow_seq(&seq_arr, &seq_cap, seq_len + 1);
            seq_arr[seq_len++] = last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */
    ensure_terms(n);
    return (n-1 < seq_len) ? seq_arr[n-1] : 0;
}
