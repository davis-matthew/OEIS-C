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
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: full, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code relies on the utility is_prime_big from
   "../utils/is_prime_big.h". */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* encode the 5 gaps (each in {2,4,6}) as an index 0…242 */
static inline unsigned pattern_index(seq_t g0, seq_t g1, seq_t g2,
                                    seq_t g3, seq_t g4)
{
    unsigned idx = 0;
    unsigned pow3 = 81;   /* 3^4 */

    idx += (unsigned)((g0 - (seq_t)2) / (seq_t)2) * pow3;
    pow3 /= 3;
    idx += (unsigned)((g1 - (seq_t)2) / (seq_t)2) * pow3;
    pow3 /= 3;
    idx += (unsigned)((g2 - (seq_t)2) / (seq_t)2) * pow3;
    pow3 /= 3;
    idx += (unsigned)((g3 - (seq_t)2) / (seq_t)2) * pow3;
    pow3 /= 3;
    idx += (unsigned)((g4 - (seq_t)2) / (seq_t)2);
    return idx;
}

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
