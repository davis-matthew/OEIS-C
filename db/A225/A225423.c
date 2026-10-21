/*
 * A225423  Primes p such that p + 70000000 is also prime.
 * https://oeis.org/A225423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 731, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: primes p such that p + 70000000 is also prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* ------------- 64‑bit deterministic Miller–Rabin ------------- */
/* ------------------------------------------------------------ */
static uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t pow_mod_u64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1U)
            r = mul_mod_u64(r, a, mod);
        a = mul_mod_u64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t a = small_primes[i] % n;
        if (a == 0)
            return true;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* ------------- Integer square root (unsigned 64 bit) ------- */
/* ------------------------------------------------------------ */
static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------ */
/* ------------- Sieve to produce primes up to a limit --------- */
/* ------------------------------------------------------------ */
static void sieve_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
    if (!is_composite) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    idx_t sqrt_lim = isqrt_u64(limit);
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (!is_composite[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[(size_t)j] = 1;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i])
            ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_composite);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i])
            pr[k++] = (uint64_t)i;

    free(is_composite);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* ------------------------------------------------------------ */
/* --------------------- Sequence Implementation ---------------- */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;     /* primes up to current limit */
static size_t     prime_cnt  = 0;       /* number of primes in list */
static idx_t      sieve_limit = 5000000;/* starting sieve limit */

static seq_t *found_primes = NULL;      /* memoised primes satisfying the condition */
static size_t found_cnt   = 0;          /* number of stored primes */
static size_t found_cap   = 0;          /* capacity of found_primes array */

static idx_t   last_checked_prime_index = 0;   /* index of next prime to examine */

/* grow an array storing seq_t values */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* append value to seq_t array */
static void append_seq_arr(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    grow_seq_arr(arr, cap, *len + 1);
    (*arr)[*len] = val;
    ++(*len);
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                     /* offset is 1; behaviour for n<1 is irrelevant */

    /* initialise once */
    if (prime_list == NULL) {
        sieve_primes_upto(sieve_limit, &prime_list, &prime_cnt);
        found_cap = 16;
        found_primes = (seq_t *)malloc(found_cap * sizeof(seq_t));
    }

    /* keep generating until we have at least n values */
    while (found_cnt < (size_t)n) {
        /* examine primes up to the current sieve limit */
        while (last_checked_prime_index < prime_cnt) {
            uint64_t p = prime_list[last_checked_prime_index++];
            uint64_t q = p + 70000000ULL;
            if (is_prime_uint64(q))
                append_seq_arr(&found_primes, &found_cnt, &found_cap, (seq_t)p);
            if (found_cnt >= (size_t)n)
                break;
        }
        if (found_cnt >= (size_t)n)
            break;   /* got enough */

        /* need more primes: enlarge sieve */
        idx_t old_limit = sieve_limit;
        sieve_limit *= 2;   /* double the limit each time */
        free(prime_list);
        sieve_primes_upto(sieve_limit, &prime_list, &prime_cnt);
        /* we have already processed primes[0..old_limit-1] */
        /* last_checked_prime_index is already set to old_cnt, continue */
    }

    return found_primes[n - 1];
}
