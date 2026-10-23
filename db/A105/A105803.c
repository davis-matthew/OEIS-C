/*
 * A105803  Self-describing sequence: 4 composite numbers, then 6 prime
 *          numbers, then 8 composites, then 9 primes, then 11 composites,
 *          etc.
 * https://oeis.org/A105803
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   sequence a(n) is given by blocks: * 4 composites, 6 primes, 8
 *            composites, 9 primes, 11 composites, 12 primes, … * Composite
 *            block lengths increase alternately by +4 and +3, * Prime block
 *            lengths increase by +3 each time
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Trial‑division primality test – sufficient for n up to ≈ 300 000. */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if ((n % d) == 0ULL) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                 /* offset – not used */

    /* static state of the generated sequence */
    static seq_t *seq = NULL;
    static idx_t seq_cap = 0, seq_size = 0;

    /* current number to test */
    static uint64_t cur_num = 4;
    /* block parameters */
    static bool in_composite = true;      /* true → composite block, false → prime block */
    static idx_t comp_len = 4;            /* length of current composite block */
    static idx_t prime_len = 6;           /* length of current prime block */
    static bool comp_inc_flag = true;     /* next composite increment: +4 if true, +3 otherwise */
    static idx_t rem = 4;                 /* remaining slots in the current block */

    /* allocate array once */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* desired zero‑based index in our array (a(4) → index 0) */
    idx_t target = n - 4;

    /* generate until we have the required element */
    while (seq_size <= target) {
        /* iterate through integers until one matches the current block type */
        while (true) {
            uint64_t val = cur_num++;
            bool p = is_prime64(val);

            if ((in_composite && !p) || (!in_composite && p)) {
                /* append the value */
                if (seq_size == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_size++] = (seq_t)val;

                /* update block bookkeeping */
                if (--rem == 0) {
                    if (in_composite) {                 /* finished a composite block */
                        /* prepare length for the next composite block */
                        comp_len += comp_inc_flag ? 4 : 3;
                        comp_inc_flag = !comp_inc_flag;
                        in_composite = false;           /* switch to prime block */
                        rem = prime_len;                /* prime block uses current length */
                    } else {                            /* finished a prime block */
                        /* prepare length for the next prime block */
                        prime_len += 3;                 /* primes always increase by 3 */
                        in_composite = true;            /* switch to composite block */
                        rem = comp_len;                 /* composite block uses updated length */
                    }
                }
                break; /* ready to generate next sequence element */
            }
        }
    }

    return seq[target];
}
