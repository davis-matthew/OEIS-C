/*
 * A040104  First ten consecutive primes which are emirps.
 * https://oeis.org/A040104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
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

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
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

/* Reverse the decimal representation of a uint64_t number.
   The result is returned as `seq_t`; if `seq_t` is too small
   the value will be truncated – the caller is responsible
   for using types large enough to hold the numbers of interest. */
static seq_t rev64_to_seq(uint64_t n)
{
    uint64_t rev = 0;
    while (n != 0)
    {
        rev = rev * 10 + (n % 10);
        n  /= 10;
    }
    return (seq_t)rev;
}

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
                    mem_move(consec_buf, consec_buf + 1, 9 * sizeof(seq_t));
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
