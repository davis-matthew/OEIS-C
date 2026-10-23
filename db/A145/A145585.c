/*
 * A145585  a(n) = number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^7.
 * https://oeis.org/A145585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   this sequence counts the number of integers removed at step n of
 *            the Eratosthenes sieve applied to N = 128 (2^7). Computed by
 *            simulating the sieve
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t counts[64];
static idx_t num_steps = 0;
static bool ready = false;

static void init_counts(void)
{
    if (ready) return;
    bool is_prime[129];
    for (idx_t i = 0; i <= 128; ++i)
        is_prime[i] = (i >= 2);
    for (idx_t i = 2; i * i <= 128; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= 128; j += i)
                is_prime[j] = false;

    bool removed[129] = { false };
    num_steps = 0;
    for (idx_t p = 2; p <= 128; ++p)
        if (is_prime[p]) {
            seq_t cnt = 0;
            for (idx_t k = p * 2; k <= 128; k += p)
                if (!removed[k]) {
                    removed[k] = true;
                    ++cnt;
                }
            counts[num_steps++] = cnt;
        }
    ready = true;
}

seq_t a(idx_t n)
{
    init_counts();
    if (n == 0) return 0;                 /* undefined offset, return 0 */
    if (n - 1 < num_steps) return counts[n - 1];
    return 0;                             /* beyond defined terms, return 0 */
}
