/*
 * A040104  First ten consecutive primes which are emirps.
 * https://oeis.org/A040104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime, is_prime_big, rev64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for the first run of ten consecutive
   emirp primes.  It returns the n‑th prime in that run
   (1 ≤ n ≤ 10).  For n > 10 it returns 0.  The search is performed
   once (on the first call) and the result is memoised.  The
   algorithm tests each prime in ascending order, keeps a sliding
   window of consecutive emirps, and stops when the window contains
   ten elements.  It uses the supplied utilities:
   - is_prime(idx_t)      for primality of the candidate
   - is_prime_big(seq_t)  for primality of the decimal reverse
   - reverse of a decimal integer is computed locally. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/rev64_to_seq.h"

/* Storage for the discovered run */
static seq_t run[10];
static bool run_found = false;

/* Find the first run of ten consecutive emirps and store it in `run`. */
static void find_run(void)
{
    if (run_found)
        return;

    idx_t cand = 2;                     /* current integer to test */
    size_t consec_len = 0;              /* number of consecutive emirps */
    seq_t consec_buf[10];               /* sliding window of recent emirps */
    size_t i;

    while (true)
    {
        if (is_prime(cand))
        {
            seq_t rev = rev64_to_seq(cand);
            if (rev != (seq_t)cand && is_prime_big(rev))
            {
                /* emirp prime discovered */
                if (consec_len < 10)
                {
                    consec_buf[consec_len++] = (seq_t)cand;
                }
                else
                {
                    /* slide window */
                    memmove(consec_buf, consec_buf + 1,
                            9 * sizeof(seq_t));
                    consec_buf[9] = (seq_t)cand;
                }

                if (consec_len == 10)
                {
                    /* copy the window to the output array */
                    for (i = 0; i < 10; ++i)
                        run[i] = consec_buf[i];
                    run_found = true;
                    return;
                }
            }
            else
            {
                /* prime but not an emirp – streak broken */
                consec_len = 0;
            }
        }
        cand++;   /* next integer */
    }
}

seq_t a(idx_t n)
{
    /* perform the computation once */
    find_run();

    if (n == 0 || !run_found)
        return (seq_t)0;

    if (n <= 10)
        return run[n - 1];

    return (seq_t)0;
}
