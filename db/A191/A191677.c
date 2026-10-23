/*
 * A191677  Numbers n such that 1^(n-1)+2^(n-1)+...+n^(n-1) == 0 (mod n).
 * https://oeis.org/A191677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth integer m such that
 *          Σ_{k=1}^{m} k^{m-1} ≡ 0 (mod m).
 * This implemen- tation generates the sequence on demand
 * using a number‑theoretic test derived from the structure
 * of the modulus.  For every candidate m it checks:
 *   – m == 1  → true
 *   – m divisible by 4 → true
 *   – otherwise it has to be an odd composite and for
 *     every odd prime divisor p of m we require (m-1) % (p-1) != 0.
 * The function builds the list incrementally; subsequent calls
 * are fast because the list is memoised.
 */

#include <stdlib.h>
#include <stdbool.h>

/* global state ------------------------------------------------------- */
static seq_t *seq = NULL;       /* built sequence               */
static size_t seq_cap = 0;      /* capacity of seq              */
static size_t seq_len = 0;      /* number of elements in seq    */
static idx_t  candidate = 0;    /* last integer examined        */
static idx_t *spf  = NULL;      /* smallest prime factor array  */
static idx_t  spf_limit = 0;    /* maximum index spf covers */

/* initialise SPF table up to 'limit' (inclusive) -------------------- */
static void init_spf(idx_t limit)
{
    if (limit <= spf_limit) return;          /* already done    */

    /* allocate or re‑allocate the array */
    spf = (idx_t*)realloc(spf, (size_t)(limit + 1) * sizeof(idx_t));

    if (spf == NULL) exit(1); /* out of memory */

    /* clear and fill */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    if (limit >= 0) spf[0] = 0;
    if (limit >= 1) spf[1] = 1;

    /* classic sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {                      /* i is prime      */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* test whether integer m satisfies the defining congruence --------- */
static bool valid_candidate(idx_t m)
{
    if (m == 1) return true;                    /* 1 is always in */
    if ((m & 3) == 0) return true;              /* multiples of 4  */

    /* even but not a multiple of 4 → fails */
    if ((m & 1) == 0) return false;

    /* odd case: require that every odd prime divisor p of m does not
     * divide m-1 in the sense that (m-1) % (p-1) != 0.               */
    idx_t t = m;
    while (t > 1) {
        idx_t p = (t <= spf_limit) ? spf[t] : t;  /* ensure safety  */
        if (p == 0) p = t;                        /* should not happen  */
        /* remove all powers of p from t */
        while (t % p == 0) t /= p;

        /* p == 2 cannot appear because m is odd */
        if ((p != 2) && ((m - 1) % (p - 1) == 0))
            return false;                        /* fails the test    */
    }
    return true;                                  /* passes all tests   */
}

/* ensure the sequence has at least 'need' terms --------------------- */
static void build_up_to(idx_t need)
{
    if (seq_len >= (size_t)need) return;

    /* enlarge SPF if needed before the first check */
    if (candidate == 0 && need > 0) init_spf(1000);

    while (seq_len < (size_t)need) {
        /* advance to the next integer */
        candidate++;

        /* enlarge SPF table when we reach beyond its current range */
        if (candidate > spf_limit) init_spf(candidate * 2 + 10);

        if (valid_candidate(candidate)) {
            /* expand the sequence array if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
                seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                if (seq == NULL) exit(1); /* out of memory */
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)candidate;
        }
    }
}

/* public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe */

    /* build the sequence up to n */
    build_up_to(n);

    return seq[n - 1];
}
