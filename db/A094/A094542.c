/*
 * A094542  Denominator of Product_{k=0..n} ((2*k+1)/(2*k+2))^((-1)^t(k))
 *          where t(k)=A010060(k) (Thue-Morse sequence).
 * https://oeis.org/A094542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of \prod_{k=0}^{n}
 *            ((2k+1)/(2k+2))^((-1)^t(k)) where t(k) is the Thue–Morse
 *            sequence (parity of set bits of k)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 23, u64 n <= 53, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses a simple sieve to pre‑compute
 *  smallest prime factors up to 2*n+2 and an array that maps
 *  a prime to its index in the prime list.  The difference
 *  between numerator and denominator prime exponents is
 *  accumulated in an array ‘diff’.  The denominator is then
 *  reconstructed from the negative entries of this array.  */

#include <stdint.h>
#include <stddef.h>

#define MAX_LIMIT 20005          /* enough for n <= 10000 (2*n+2 <= 20002) */

static unsigned int spf[MAX_LIMIT];     /* smallest prime factor */
static int prime_index[MAX_LIMIT];      /* index of a prime in primes[] */
static int primes[MAX_LIMIT];           /* list of primes */
static int primes_cnt = 0;              /* number of primes stored */
static unsigned int sieve_limit = 0;    /* largest value for which sieve was built */

/* ------------------------------------------------------------------ */
/*  Simple linear sieve to fill spf[] and primes[] up to ‘limit’.      */
/*  It also records the index of each prime in the primes[] array.     */
/* ------------------------------------------------------------------ */
static void build_sieve(unsigned int limit)
{
    if (limit <= sieve_limit) return;        /* already built */

    for (unsigned int i = 2; i <= limit; ++i) {
        if (!spf[i]) {                      /* i is prime */
            spf[i] = i;
            prime_index[i] = primes_cnt;
            primes[primes_cnt++] = (int)i;
        }
        for (int j = 0; j < primes_cnt &&
               (unsigned int)primes[j] <= spf[i] &&
               (unsigned long long)i * primes[j] <= limit; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Update the exponent difference array with the factorisation of ‘num’.      */
/*  The sign ‘s’ is +1 when the factor contributes to the numerator,      */
/*  –1 when it contributes to the denominator.                 */
/* ------------------------------------------------------------------ */
static void add_factors(int num, int s, int *diff)
{
    while (num > 1) {
        int p = spf[num];
        int cnt = 0;
        while (num % p == 0) {
            num /= p;
            ++cnt;
        }
        diff[prime_index[p]] += s * cnt;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Ensure we have all primes needed for 2*n+2.  */
    /* ------------------------------------------------------------------ */
    unsigned int limit = (unsigned int)(2 * n + 2);
    if (limit >= MAX_LIMIT) {               /* guard against overflow */
        limit = MAX_LIMIT - 1;
    }
    build_sieve(limit);

    /* ------------------------------------------------------------------ */
    /*  Prepare exponent‑difference array.  */
    /* ------------------------------------------------------------------ */
    static int diff[MAX_LIMIT];
    for (int i = 0; i < primes_cnt; ++i) diff[i] = 0;

    /* ------------------------------------------------------------------ */
    /*  Iterate over k = 0 … n.  */
    /* ------------------------------------------------------------------ */
    for (idx_t k = 0; k <= n; ++k) {
        int a_num = (int)(2 * k + 1);        /* 2k+1  */
        int b_num = (int)(2 * k + 2);        /* 2k+2  */
        int parity = __builtin_parityll((unsigned long long)k); /* 0 = even, 1 = odd */
        int sign = parity ? -1 : 1;          /* exponent (+1 or –1) */

        if (sign == 1) {                    /* multiply by (a/b) */
            add_factors(a_num,  +1, diff);   /* numerator gets a */
            add_factors(b_num,  -1, diff);   /* denominator gets b */
        } else {                            /* multiply by (b/a) */
            add_factors(a_num,  -1, diff);   /* denominator gets a */
            add_factors(b_num,  +1, diff);   /* numerator gets b */
        }
    }

    /* ------------------------------------------------------------------ */
    /*  Build the denominator from negative exponent differences.  */
    /* ------------------------------------------------------------------ */
    seq_t denom = (seq_t)1;
    for (int i = 0; i < primes_cnt; ++i) {
        int e = diff[i];
        if (e < 0) {
            int exp = -e;
            int p = primes[i];
            for (int j = 0; j < exp; ++j) {
                denom *= (seq_t)p;
            }
        }
    }
    return denom;
}
