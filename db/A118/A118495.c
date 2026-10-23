/*
 * A118495  Chen primes written backwards.
 * https://oeis.org/A118495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chen primes written backwards: a(n) is the decimal reversal of the n‑th Chen prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"
#include "../utils/grow_seq.h"

static idx_t current_candidate = 2;
static seq_t *chen_rev = NULL;
static size_t chen_cnt = 0;
static size_t chen_cap = 0;

/* Reverse the decimal digits of an integer.  Result fits in seq_t. */
static seq_t rev_digits(idx_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (chen_cnt < n) {
        /* Find next prime p. */
        while (!is_prime_big((seq_t)current_candidate))
            current_candidate++;

        idx_t p = current_candidate;
        idx_t q = p + 2;

        bool chen = false;
        if (is_prime_big((seq_t)q)) {
            chen = true;
        } else {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64((uint64_t)q, primes, exps, &cnt);
            int total = 0;
            for (size_t i = 0; i < cnt; ++i)
                total += (int)exps[i];
            if (total == 2)
                chen = true;
        }

        if (chen) {
            seq_t rev = rev_digits(p);
            if (chen_cnt >= chen_cap)
                grow_seq(&chen_rev, &chen_cap, chen_cnt + 1);
            chen_rev[chen_cnt++] = rev;
        }

        current_candidate++;
    }

    return chen_rev[n - 1];
}
