/*
 * A211515  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^3=x*y.
 * https://oeis.org/A211515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} Sum_{j=1..n} d(i*j) where d(k) is the number
 *            of divisors of k
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm counts, for each w∈[1,n], the number of pairs (x,y)
   with x≤n, y≤n and x·y = w³, and sums these counts over all w. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple prime sieve up to 10000 – once per program.                  */
#define PRIMELIM   10000
#define MAXPRIMCNT 1229        /* π(10000) */

static uint32_t primes[MAXPRIMCNT];
static size_t   prime_cnt = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool is_comp[PRIMELIM + 1] = {0};

    for (uint32_t i = 2; i <= PRIMELIM; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIMELIM) {
                for (uint32_t j = i * i; j <= PRIMELIM; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/* Context for recursive divisor enumeration of w³.                   */
typedef struct {
    uint64_t w3;          /* w³                               */
    idx_t    n;           /* search bounds for x and y        */
    const uint32_t *factors; /* distinct prime factors of w   */
    const uint8_t  *exp;     /* corresponding exponents (times 3) */
    size_t          fcnt;    /* number of distinct primes        */
    seq_t          *sum;     /* pointer to local counter         */
} DivCtx;

static void dfs_div(DivCtx *ctx, size_t idx, uint64_t cur)
{
    if (idx == ctx->fcnt) {
        /* cur is a divisor of w³ */
        if (cur <= (uint64_t)ctx->n) {
            uint64_t y = ctx->w3 / cur;
            if (y <= (uint64_t)ctx->n) {
                (*ctx->sum) += (seq_t)1;
            }
        }
        return;
    }

    uint32_t p   = ctx->factors[idx];
    uint8_t  exp = ctx->exp[idx];
    uint64_t mult = 1;

    for (uint8_t k = 0; k <= exp; ++k) {
        dfs_div(ctx, idx + 1, cur * mult);
        mult *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_primes();

    seq_t total = (seq_t)0;

    /* For each w, factorize it and explore divisors of w³. */
    for (idx_t w = 1; w <= n; ++w) {
        /* Factor w – we need at most two small arrays. */
        uint32_t  fac[10];
        uint8_t   exp[10];
        size_t    fcnt = 0;
        idx_t     tmp  = w;

        for (size_t i = 0; i < prime_cnt &&
                 (uint64_t)primes[i] * primes[i] <= (uint64_t)tmp; ++i) {
            if (tmp % primes[i] == 0) {
                uint8_t cnt = 0;
                while (tmp % primes[i] == 0) {
                    tmp /= primes[i];
                    ++cnt;
                }
                fac[fcnt]   = primes[i];
                exp[fcnt]   = cnt * 3;   /* exponent in w³            */
                ++fcnt;
            }
        }
        if (tmp > 1) {
            fac[fcnt] = (uint32_t)tmp;
            exp[fcnt] = 3;
            ++fcnt;
        }

        uint64_t w3 = (uint64_t)w * w * w;
        seq_t    local_sum = (seq_t)0;

        DivCtx ctx = { w3, n, fac, exp, fcnt, &local_sum };
        dfs_div(&ctx, 0, 1ULL);

        total += local_sum;
    }

    return total;
}
