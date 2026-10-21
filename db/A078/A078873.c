/*
 * A078873  Sorted version of A078872.
 * https://oeis.org/A078873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth prime p ≥ 7 such that the 5 gaps among the 6
 *            consecutive primes starting at p lie in {2,4,6} and this gap
 *            pattern has not occurred before. The sequence is the sorted list
 *            of the starting primes of each distinct 5‑gap pattern
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big, pattern_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code relies on the utility is_prime_big from
   "../utils/is_prime_big.h". */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/pattern_index.h"

/* maximum number of distinct patterns (3^5 = 243) – use some slack */
#define MAX_SEQ 1000

static seq_t seq_buf[MAX_SEQ];
static size_t seq_len = 0;

/* seen[pattern] == true if this 5‑gap pattern has already been recorded.
   Patterns are encoded as a base‑3 number with 2→0, 4→1, 6→2. */
static bool seen[243] = {false};

/* sliding window of the last six primes */
static seq_t last6[6];
static size_t last6_len = 0;

/* return the next prime strictly greater than p */
static inline seq_t next_prime(seq_t p)
{
    seq_t q = p + (seq_t)1;
    if (q <= (seq_t)2) return (seq_t)2;
    if ((q & (seq_t)1) == (seq_t)0) ++q;          /* make q odd */
    while (!is_prime_big(q)) q += (seq_t)2;        /* skip evens */
    return q;
}

seq_t a(idx_t n)
{
    /* nothing to do for n==0 – the OEIS offset is 1, so callers should
       use a(1), a(2), …   */
    if (n == (idx_t)0) return (seq_t)0;         /* undefined, but avoid UB */

    /* generate terms until we have at least n */
    while (seq_len < (size_t)n)
    {
        /* obtain next prime */
        seq_t p = next_prime(last6_len ? last6[last6_len - 1] : (seq_t)2);

        /* slide window of last six primes */
        if (last6_len < 6)
        {
            last6[last6_len++] = p;
            continue;   /* need a full window before checking */
        }
        else
        {
            for (size_t i = 0; i < 5; ++i) last6[i] = last6[i + 1];
            last6[5] = p;
        }

        /* check only if window starts at a prime >= 7 */
        if (last6[0] < (seq_t)7) continue;

        /* verify all five gaps are in {2,4,6} */
        bool ok = true;
        seq_t gaps[5];
        for (size_t i = 0; i < 5; ++i)
        {
            gaps[i] = last6[i + 1] - last6[i];
            if (gaps[i] != (seq_t)2 &&
                gaps[i] != (seq_t)4 &&
                gaps[i] != (seq_t)6)
            {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        /* encode pattern and test uniqueness */
        unsigned idx = pattern_index(gaps[0], gaps[1], gaps[2], gaps[3], gaps[4]);
        if (seen[idx]) continue;   /* pattern already seen – skip */

        /* new pattern – record it */
        if (seq_len < MAX_SEQ)
        {
            seq_buf[seq_len++] = last6[0];
            seen[idx] = true;
        }
        else
        {
            /* should not happen for requested ranges (n ≤ 10 000) */
            break;
        }
    }

    return seq_buf[n - 1];
}
