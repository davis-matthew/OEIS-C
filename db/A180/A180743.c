/*
 * A180743  Ascending sequence of numbers such that the sum of any two
 *          distinct elements (even + odd) is a prime number.
 * https://oeis.org/A180743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically smallest lexicographically
 *            increasing sequence of positive integers such that every sum of
 *            a pair of distinct elements of different parity is prime. The
 *            sequence is built by appending the smallest integer greater than
 *            the current maximum that satisfies the prime‑sum...
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Static storage for the sequence and its parity sub‑arrays. */
    static seq_t *seq   = NULL;   /* all terms */
    static size_t seq_cap = 0, seq_len = 0;
    static seq_t *odd  = NULL;   /* odd terms only */
    static size_t odd_cap = 0, odd_cnt  = 0;
    static seq_t *even = NULL;   /* even terms only */
    static size_t even_cap = 0, even_cnt = 0;

    /* Return a previously computed value if possible. */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* Grow the sequence until the requested index is reached. */
    while (seq_len < (size_t)n)
    {
        /* Candidate is the smallest integer larger than the current maximum. */
        seq_t cand = (seq_len == 0) ? 1 : seq[seq_len - 1] + 1;

        /* Find the smallest candidate satisfying the property. */
        for (;; ++cand)
        {
            bool ok = true;

            if ((cand & 1) == 0)            /* even candidate */
            {
                for (size_t i = 0; i < odd_cnt; ++i)
                {
                    seq_t sum = cand + odd[i];
                    if (!is_prime_big(sum))
                    {
                        ok = false;
                        break;
                    }
                }
            }
            else                            /* odd candidate */
            {
                for (size_t i = 0; i < even_cnt; ++i)
                {
                    seq_t sum = cand + even[i];
                    if (!is_prime_big(sum))
                    {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok)
                break;   /* candidate accepted */
        }

        /* Append the accepted candidate to the full sequence. */
        grow_seq(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = cand;

        /* Store the candidate in the appropriate parity array. */
        if ((cand & 1) == 0)              /* even */
        {
            grow_seq(&even, &even_cap, even_cnt + 1);
            even[even_cnt++] = cand;
        }
        else                              /* odd */
        {
            grow_seq(&odd, &odd_cap, odd_cnt + 1);
            odd[odd_cnt++] = cand;
        }
    }

    return seq[(size_t)n - 1];
}
