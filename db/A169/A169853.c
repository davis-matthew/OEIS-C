/*
 * A169853  EKG sequence started at 11 instead of 2.
 * https://oeis.org/A169853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused positive integer that shares a * common
 *            prime factor with a(n−1); a(1) = 11, a(2) = 22. * * The
 *            algorithm keeps a dynamic table of already used numbers and a *
 *            list of primes (generated once up to a fixed limit). Each step *
 *            factors the previous term into its distinct prime...
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------  static data  ---------------------------------------- */

/* sequence array */
static seq_t *seq = NULL;          /* values of the sequence */
static size_t seq_len = 0;         /* number of stored terms */
static size_t seq_cap = 0;         /* allocated capacity */

/* used table – used[i] == 1 if number i has appeared in the sequence */
static uint8_t *used = NULL;       /* boolean array */
static size_t used_cap = 0;        /* allocated size of used */

/* list of primes up to prime_limit */
static uint32_t *primes = NULL;    /* array of primes */
static size_t prime_cnt = 0;       /* number of primes */
static const uint32_t PRIME_LIMIT = 250000; /* sufficient for 10 000 terms */

/* initialization flag */
static int initialized = 0;

/* ------------  helper functions  ----------------------------------- */

/* enlarge a dynamic array of uint8_t to hold at least 'need' entries */
static void ensure_used_cap(size_t need)
{
    if (used_cap > 0 && used_cap >= need)
        return;

    size_t new_cap = used_cap ? used_cap : 1024;
    while (new_cap < need)
        new_cap <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    if (!tmp) abort();          /* out of memory */
    /* new slots are already zeroed by realloc only if size increased ... so zero them */
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* enlarge the sequence array to hold at least 'need' terms */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap > 0 && seq_cap >= need)
        return;

    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* generate list of primes up to PRIME_LIMIT using a simple sieve */
static void init_primes(void)
{
    if (primes) return;              /* already done */

    /* sieve array */
    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve) abort();

    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) continue;
        for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
            sieve[j] = 1;
    }

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) primes[idx++] = i;

    prime_cnt = cnt;
    free(sieve);
}

/* factorise n into distinct prime factors; result stored in out[], count in *cnt.
 * The caller must provide out[] large enough to hold all factors (should be
 * at most ~log₂(n)). */
static void factorize(uint64_t n, uint32_t *out, size_t *cnt)
{
    size_t k = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            out[k++] = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1) out[k++] = (uint32_t)n;  /* remaining prime factor */
    *cnt = k;
}

/* initialise static data: sequence, used table, primes */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* set up used table */
    used = (uint8_t *)calloc(1024, 1);
    used_cap = 1024;

    /* set up sequence array */
    seq = (seq_t *)malloc(64 * sizeof(seq_t));
    seq_cap = 64;
    seq_len = 0;

    /* insert the first two terms */
    seq[seq_len++] = (seq_t)11;
    ensure_used_cap((size_t)11 + 1);
    used[11] = 1;
    seq[seq_len++] = (seq_t)22;
    ensure_used_cap((size_t)22 + 1);
    used[22] = 1;

    /* initialise prime list */
    init_primes();
}

/* ensure that the sequence has at least n terms stored */
static void extend_sequence(idx_t n)
{
    /* already have enough */
    if (seq_len >= (size_t)n) return;

    /* temporary buffer for prime factors of the previous term;
       we expect very few factors, so 16 slots are plenty. */
    uint32_t pf[16];
    size_t pf_cnt = 0;

    while (seq_len < (size_t)n) {
        seq_t last = seq[seq_len - 1];
        uint64_t last_val = (uint64_t)last;

        if (last_val >= (uint64_t)UINT32_MAX) {
            /* Very large last term – fall back to trial division by 2..sqrt */
            pf_cnt = 0;
            uint64_t tmp = last_val;
            for (uint64_t p = 2; p * p <= tmp; ++p) {
                if (tmp % p == 0) {
                    pf[pf_cnt++] = (uint32_t)p;
                    while (tmp % p == 0) tmp /= p;
                }
            }
            if (tmp > 1) pf[pf_cnt++] = (uint32_t)tmp;
        } else {
            factorize((uint64_t)last_val, pf, &pf_cnt);
        }

        /* find the next term: smallest unused number sharing a prime factor */
        seq_t best = 0;
        int has_best = 0;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint32_t p = pf[i];
            uint64_t cand = p;          /* start from the prime itself */

            /* scan forward by steps of p until we find an unused number */
            for (;;) {
                ensure_used_cap((size_t)cand + 1);
                if (!used[(size_t)cand])
                    break;
                cand += p;
            }

            if (!has_best || cand < (uint64_t)best) {
                best = (seq_t)cand;
                has_best = 1;
            }
        }

        /* mark the chosen number as used and append to the sequence */
        size_t idx = (size_t)best;
        ensure_used_cap(idx + 1);
        used[idx] = 1;
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = best;
    }
}

/* ------------  public function  ------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but guard anyway */

    init_sequence();
    extend_sequence(n);
    return seq[(size_t)n - 1];
}
