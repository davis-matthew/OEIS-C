/*
 * A132256  Isolated primes congruent to {1, 29} mod 30.
 * https://oeis.org/A132256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth isolated prime congruent to 1 or 29 (mod 30)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, next_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_candidate.h"

static seq_t *isolated = NULL;          /* array of isolated primes */
static size_t isolated_cnt = 0;         /* number of stored elements */
static size_t isolated_cap = 0;         /* allocated capacity */

static idx_t tested_candidate = 2;      /* next number to test */

/* Grow the isolated array if necessary */
static void ensure_isolated_capacity(void)
{
    if (isolated_cnt >= isolated_cap) {
        isolated_cap = isolated_cap ? isolated_cap * 2 : 4;
        isolated = (seq_t *)realloc(isolated, isolated_cap * sizeof(seq_t));
    }
}

/* Generate isolated primes until we have at least 'target' entries. */
static void add_isolated_until(idx_t target)
{
    while (isolated_cnt < target) {
        if (is_prime_big((seq_t)tested_candidate)) {
            /* Check modulus 30 constraint */
            if (tested_candidate % 30 == 1 || tested_candidate % 30 == 29) {
                bool isolated_ok = true;

                /* p-2 */
                if (tested_candidate >= 3 &&
                    is_prime_big((seq_t)(tested_candidate - 2)))
                    isolated_ok = false;
                /* p-1 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate - 1)))
                    isolated_ok = false;
                /* p+1 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate + 1)))
                    isolated_ok = false;
                /* p+2 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate + 2)))
                    isolated_ok = false;

                if (isolated_ok) {
                    ensure_isolated_capacity();
                    isolated[isolated_cnt++] = (seq_t)tested_candidate;
                }
            }
        }
        tested_candidate = next_candidate(tested_candidate);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }
    add_isolated_until(n);
    return isolated[n - 1];
}
