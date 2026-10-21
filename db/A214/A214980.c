/*
 * A214980  Positions of zeros in A214979.
 * https://oeis.org/A214980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the positions of zeros in A214979 (A214980).
   a(n) returns the n‑th integer k for which the cumulative
   difference V(k)–U(k) equals zero.  V(k) is the total number of
   terms in the Zeckendorf (Fibonacci) representations of 1,…,k,
   U(k) is the total number of terms in the Lucas representations
   of 1,…,k.  The function memoises intermediate results and
   works for any build where seq_t is an unsigned integer type
   or an arbitrary‑precision integer class, and idx_t is uint64_t. */

#include <stdlib.h>

static seq_t *fib      = NULL;
static size_t fib_len  = 0;
static size_t fib_cap  = 0;

static seq_t *lucas    = NULL;
static size_t lucas_len  = 0;
static size_t lucas_cap  = 0;

static void ensure_fib(idx_t limit)
{
    if (fib_len == 0) {
        fib_cap = 4;
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        fib[0] = 1;
        fib[1] = 1;
        fib_len = 2;
    }
    while (fib[fib_len - 1] < (seq_t)limit) {
        if (fib_len == fib_cap) {
            fib_cap <<= 1;
            fib = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
        }
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

static void ensure_lucas(idx_t limit)
{
    if (lucas_len == 0) {
        lucas_cap = 4;
        lucas = (seq_t*)malloc(lucas_cap * sizeof(seq_t));
        lucas[0] = 2;
        lucas[1] = 1;
        lucas_len = 2;
    }
    while (lucas[lucas_len - 1] < (seq_t)limit) {
        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;
            lucas = (seq_t*)realloc(lucas, lucas_cap * sizeof(seq_t));
        }
        lucas[lucas_len] = lucas[lucas_len - 1] + lucas[lucas_len - 2];
        ++lucas_len;
    }
}

static size_t find_max_fib(seq_t rem)
{
    size_t lo = 0, hi = fib_len - 1;
    while (lo < hi) {
        size_t mid = (lo + hi + 1) >> 1;
        if (fib[mid] <= rem)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static idx_t count_fib_terms(idx_t n)
{
    seq_t rem = (seq_t)n;
    idx_t cnt = 0;
    while (rem > 0) {
        size_t idx = find_max_fib(rem);
        rem -= fib[idx];
        ++cnt;
    }
    return cnt;
}

static idx_t count_lucas_terms(idx_t n)
{
    seq_t rem = (seq_t)n;
    idx_t cnt = 0;
    while (rem > 0) {
        seq_t best = 0;
        for (size_t i = 0; i < lucas_len; ++i) {
            seq_t val = lucas[i];
            if (val <= rem && val > best)
                best = val;
        }
        rem -= best;
        ++cnt;
    }
    return cnt;
}

static seq_t *zeros = NULL;
static size_t zeros_len = 0;
static size_t zeros_cap = 0;
static idx_t processed_n = 0;
static long long diff = 0;

static void generate_zeros_up_to(idx_t goal)
{
    if (zeros == NULL) {
        zeros_cap = 4;
        zeros = (seq_t*)malloc(zeros_cap * sizeof(seq_t));
    }
    while (zeros_len < goal) {
        ++processed_n;
        ensure_fib(processed_n);
        ensure_lucas(processed_n);

        idx_t v = count_fib_terms(processed_n);
        idx_t u = count_lucas_terms(processed_n);

        diff += (long long)v - (long long)u;

        if (diff == 0) {
            if (zeros_len == zeros_cap) {
                zeros_cap <<= 1;
                zeros = (seq_t*)realloc(zeros, zeros_cap * sizeof(seq_t));
            }
            zeros[zeros_len++] = (seq_t)processed_n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_zeros_up_to(n);
    return zeros[n - 1];
}
