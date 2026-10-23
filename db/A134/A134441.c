/*
 * A134441  Last two digits of primes of form 4n+1 (A002144), excluding 5.
 *          Leading 0's omitted.
 * https://oeis.org/A134441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: unique last two decimal digits of primes ≡ 1 (mod 4),
   excluding the prime 5, sorted ascending.  Leading 0's are omitted.
   The sequence is finite (20 terms).  For n > 20 the function
   returns 0. */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
 
/* Array to hold the 20 terms. */
static seq_t _seq[20];
static idx_t _seq_len = 0;
static bool _seq_built = false;
 
/* Build the sequence once. */
static void _build_seq(void)
{
    if (_seq_built)
        return;
 
    /* A limit large enough that all 20 residues appear. */
    const idx_t LIMIT = 50000;
    bool *comp = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!comp)  /* Should never happen in the test harness. */
        return;
 
    /* Simple sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                comp[j] = true;
 
    bool seen[100] = { false };
    idx_t seen_cnt = 0;
 
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (comp[p] == false && (p % 4 == 1)) {  /* p ≡ 1 (mod 4) */
            if (p == 5)               /* skip the prime 5 */
                continue;
            idx_t r = p % 100;       /* last two digits */
            if (!seen[r]) {
                seen[r] = true;
                ++seen_cnt;
                if (seen_cnt == 20)  /* all residues found */
                    break;
            }
        }
    }
    free(comp);
 
    /* Store the seen residues in ascending order. */
    idx_t k = 0;
    for (idx_t i = 0; i < 100; ++i)
        if (seen[i])
            _seq[k++] = (seq_t)i;
    _seq_len = k;
    _seq_built = true;
}
 
/* Public function required by the build. */
seq_t a(idx_t n)
{
    _build_seq();
    if (n == 0 || n > _seq_len)
        return 0;
    return _seq[n - 1];
}
