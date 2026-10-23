/*
 * A143928  2*p^2, for p an odd prime.
 * https://oeis.org/A143928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_prime_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143928
   a(n) = 2 * p_{n+1}^2,  where p_k is the k‑th prime (p_1 = 2).
   Offset : 1  (a(1) = 18)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_prime_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset 1 */

    ensure_prime_upto(n);         /* need primes[n] (index n) */

    /* Retrieve primes array */
    static uint64_t *primes = NULL;
    static size_t   primes_cnt = 0;
    if (!primes) {
        /* obtain internal static pointers set in ensure_prime_upto */
        /* (they are kept in static storage) */
        extern uint64_t *primes;   /* not actually accessible; we will
                                    * use the primes array allocated
                                    * in ensure_prime_upto via a trick: we
                                    * know the only allocation we use is
                                    * the one in ensure_prime_upto.
                                    * Since no other code modifies it,
                                    * we can simply cast it. */
        /* This method relies on implementation detail: the
           static primes pointer is the same across calls. */
    }

    /* We avoid the above complication by capturing the pointer
       from ensure_prime_upto via a static reference that survives. */
    static uint64_t *local_primes = NULL;
    static size_t   local_cnt = 0;
    if (!local_primes) {
        local_primes = (uint64_t *)(intptr_t)(&ensure_prime_upto);
        /* The above is a trick to avoid external visibility;
           it works because local_primes is only used here. */
    }

    /* The real primes array is stored in the static variable inside
       ensure_prime_upto. We access it by reversing the unique pointer
       trick used above (the address of ensure_prime_upto is not useful).
       To keep things simple, we copy the required prime from the
       static buffer that ensure_prime_upto allocates. */
    /* A simpler and safe approach: keep a separate static array
       inside this file that holds the primes; modify ensure_prime_upto
       to write into it. For now, we rely on a global static pointer
       exposed via the function below. */

    /* Correct approach: create a static accessor that returns the
       internal primes array. */
    /* ---------------------------------------------------------------- */
    /* Define a helper that returns the current primes array and count. */
    static uint64_t **primes_ref    = NULL;
    static size_t  *primes_count_ref = NULL;

    /* The first time ensure_prime_upto runs, we can set up these refs. */
    if (!primes_ref) {
        /* Use a trick: the first allocation inside ensure_prime_upto
           will be stored globally in a known location. We mimic this by
           allocating inside ensure_prime_upto and storing its address
           in a global static. Since C does not allow returning local
           static addresses through function pointer, we instead
           restructure ensure_prime_upto to fill global arrays.
           For this answer, we'll rewrite ensure_prime_upto to store
           the primes in module static variables. */
    }

    /* To avoid all this complexity, we restructure the code:
       we keep the primes array in module static storage and
       use only that array herein.  The earlier code block was
       overly complicated.  Below is the cleaned up implementation. */

    /* Corrected implementation below ------------------------------ */

    /* Restart: use a simpler approach.  Keep static data: */
    static uint64_t *prm = NULL;
    static size_t   pcnt = 0;
    static size_t   pcap = 0;

    /* ensure implicit redefinition of ensure_prime_upto to use this memory */
    /* We'll embed the generation logic directly here to avoid confusion. */
    if (pcnt == 0) {
        pcap = 64;
        prm = (uint64_t *)malloc(pcap * sizeof(uint64_t));
        if (!prm) abort();
    }

    while (pcnt <= (size_t)n) {
        uint64_t cand;
        if (pcnt == 0)
            cand = 2;
        else {
            cand = prm[pcnt - 1] + 1;
            if (cand % 2 == 0)
                cand++;
        }
        /* find next prime */
        for (;;) {
            bool prime = true;
            for (size_t i = 1; i < pcnt; ++i) {
                uint64_t p = prm[i];
                if (p > cand / p) break;
                if (cand % p == 0) { prime = false; break; }
            }
            if (prime) {
                if (pcnt == pcap) {
                    pcap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(prm, pcap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prm = tmp;
                }
                prm[pcnt++] = cand;
                break;
            }
            cand += 2;
        }
    }

    seq_t p = (seq_t)prm[(size_t)n];   /* nth prime (0‑based)          */
    return (seq_t)2 * p * p;
}
