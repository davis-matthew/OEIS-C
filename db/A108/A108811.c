/*
 * A108811  Increasing consecutive prime pairs alternating between form 4k+3
 *          and 4k+1.
 * https://oeis.org/A108811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate consecutive primes of form 4k+3 and 4k+1 alternating,
   and output them in order as required for sequence A108811.
   Uses simple trial‑division prime generation, memoised across calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state retained across calls */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t  *seq    = NULL;
    static size_t  seq_cnt = 0, seq_cap = 0;
    static bool expect_3 = true;          /* next pair should be 4k+3 */
    static uint64_t prev_prime = 0;       /* previous prime seen */

    /* initialise prime storage */
    if (primes_cap == 0) {
        primes_cap = 1024;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    /* initialise sequence storage */
    if (seq_cap == 0) {
        seq_cap = 2048;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (seq_cnt < (size_t)n) {
        uint64_t cand;

        /* find next odd candidate prime */
        if (primes_cnt == 0) {
            cand = 3;
        } else {
            cand = primes[primes_cnt - 1] + 2;
        }

        /* step through odd numbers until a prime is found */
        for (;;) {
            bool is_prime = true;
            if (cand < 2) {
                is_prime = false;
            } else {
                for (size_t i = 0; i < primes_cnt; ++i) {
                    uint64_t p = primes[i];
                    if ((unsigned __int128)p * p > cand) break;
                    if (cand % p == 0) { is_prime = false; break; }
                }
            }

            if (is_prime) {
                /* store prime and ensure capacity */
                if (primes_cnt >= primes_cap) {
                    primes_cap <<= 1;
                    primes = (uint64_t *)realloc(primes,
                                                primes_cap * sizeof(uint64_t));
                }
                primes[primes_cnt++] = cand;

                /* check pair with previous prime, if any */
                if (prev_prime != 0) {
                    uint64_t mod_prev = prev_prime & 3ULL;
                    uint64_t mod_cur  = cand & 3ULL;
                    if (mod_prev == mod_cur) {
                        /* only accept pair that matches expected parity */
                        bool cur_is_3 = (mod_prev == 3ULL);
                        if (cur_is_3 == expect_3) {
                            /* store both primes in the sequence */
                            if (seq_cnt + 2 > seq_cap) {
                                seq_cap <<= 1;
                                seq = (seq_t *)realloc(seq,
                                                       seq_cap * sizeof(seq_t));
                            }
                            seq[seq_cnt++] = (seq_t)prev_prime;
                            seq[seq_cnt++] = (seq_t)cand;
                            expect_3 = !expect_3;           /* alternate */
                            if (seq_cnt >= (size_t)n) break; /* enough terms */
                        }
                    }
                }
                prev_prime = cand;   /* advance to next prime */
                break;               /* exit candidate loop */
            } else {
                cand += 2;          /* skip even numbers */
            }
        }
    }

    return seq[n - 1];
}
