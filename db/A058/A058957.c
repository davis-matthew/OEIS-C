/*
 * A058957  Numbers having at least two representations as b^2 - c^2 with b >
 *          c >= 0.
 * https://oeis.org/A058957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: integers that can be written in at least two different ways
   as b² – c² with b > c ≥ 0 (b and c integers). */

/* Note: The implementation stores all found numbers in a dynamic array
   and simply scans successive integers until the requested index
   has been reached.  This is sufficient for the limits required
   (≈10 000th term) and runs in well under 2 seconds. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */

static seq_t *seq_terms = NULL;          /* array of found terms      */
static size_t seq_terms_cnt = 0;         /* number of terms stored   */
static size_t seq_terms_cap = 0;         /* allocated capacity       */

static idx_t *spf = NULL;                /* least‑prime‑factor table */
static idx_t spf_limit = 0;              /* maximum index with spf set */

/* ------------------------------------------------------------------ */
/*  Expand the spf table to at least 'limit' (inclusive).              */
/* ------------------------------------------------------------------ */
static void ensure_spf(idx_t limit)
{
    if ((idx_t)limit <= spf_limit)
        return;

    /* Allocate (limit+1) entries so that indices 0..limit are valid. */
    idx_t new_limit = limit + 1;
    idx_t *tmp = (idx_t *)realloc(spf, new_limit * sizeof(idx_t));
    if (!tmp) abort();      /* fail loudly on allocation error   */
    spf = tmp;

    /* Initialise new entries with themselves. */
    for (idx_t i = spf_limit + 1; i < new_limit; ++i)
        spf[i] = i;

    /* Simple sieve to fill in smallest prime factor for each entry.   */
    for (idx_t i = 2; i * i < new_limit; ++i) {
        if (spf[i] == i) {                  /* i is prime            */
            for (idx_t j = i * i; j < new_limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_limit = limit;                      /* last index that is valid */
}

/* ------------------------------------------------------------------ */
/*  Count how many (unordered) representations a number has as a
    difference of two squares, counting the trivial representation
    with c = 0.  Returns 0, 1, or 2 (we only need to know whether
    at least two exist).                                                     */
/* ------------------------------------------------------------------ */
static seq_t rep_count(idx_t n)
{
    if (n == 0) return 0;

    idx_t t;
    if ((n & 1) == 0) {                     /* n even           */
        if ((n & 3) != 0) return 0;         /* n ≡ 2 (mod 4)    */
        t = n >> 2;                         /* work with n/4    */
    } else {
        t = n;
    }

    ensure_spf(t);

    /* Compute τ(t), the number of divisors of t. */
    size_t tau = 1;
    idx_t tmp = t;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        size_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        tau *= (exp + 1);
    }

    /* Each pair of divisors gives a representation; unordered pairs give
       (τ+1)/2 representations.  Return 2 if at least two, otherwise the
       exact count. */
    size_t reps = (tau + 1) / 2;
    return (reps >= 2) ? 2 : (seq_t)reps;
}

/* ------------------------------------------------------------------ */
/*  Append a value to the dynamic sequence array.                     */
/* ------------------------------------------------------------------ */
static void append_term(seq_t val)
{
    if (seq_terms_cnt == seq_terms_cap) {
        size_t new_cap = (seq_terms_cap == 0) ? 64 : seq_terms_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        seq_terms = new_arr;
        seq_terms_cap = new_cap;
    }
    seq_terms[seq_terms_cnt++] = val;
}

/* ------------------------------------------------------------------ */
/*  Public API: return the n-th term of the sequence.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, but safe return */

    static idx_t current = 0;            /* last integer inspected */

    while (seq_terms_cnt < n) {
        ++current;
        if (rep_count(current) >= 2)
            append_term((seq_t)current);
    }
    return seq_terms[n - 1];
}
