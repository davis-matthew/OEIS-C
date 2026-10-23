/*
 * A016141  Inverse of 2132nd cyclotomic polynomial.
 * https://oeis.org/A016141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A016141: Inverse of the 2132‑th cyclotomic polynomial.
   Let Φ2132(x)=Σ_{i=0}^{960} φ_i x^i.  
   The generating function of the sequence is 1/Φ2132(x); hence
   a(0)=1 and for n≥1:
       a(n) = -∑_{i=1}^{960} φ_i * a(n-i),
   where φ_i is the coefficient of x^i in Φ2132(x) (φ_0=1).
   The sequence is periodic with period 2132. */

/* ------------------------------------------------------------------ */
/* Standard headers ----------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper for memory allocation that works in both C and C++ --------- */
static seq_t *alloc_array(size_t n)
{
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

static void free_array(seq_t *p)
{
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* Polynomial division:  divides numerator by monic denominator ------- */
static void divide_in_place(seq_t **num_ptr, size_t *deg_ptr,
                            const seq_t *den, size_t den_deg)
{
    size_t num_deg = *deg_ptr;
    size_t q_deg   = num_deg - den_deg;

    seq_t *quotient = alloc_array(q_deg + 1);
    seq_t *tmp      = alloc_array(num_deg + 1);

    for (size_t i = 0; i <= num_deg; ++i) tmp[i] = (*num_ptr)[i];

    for (long i = (long)num_deg; i >= (long)den_deg; --i)
    {
        seq_t coeff = tmp[i];
        quotient[i - den_deg] = coeff;
        for (size_t j = 0; j <= den_deg; ++j)
            tmp[i - den_deg + j] -= coeff * den[j];
    }

    free_array(*num_ptr);
    *num_ptr = quotient;
    *deg_ptr = q_deg;
    free_array(tmp);
}

/* ------------------------------------------------------------------ */
/* Construct Φ2132(x) and store its coefficients --------------------- */
static void build_phi_2132(seq_t **phi_ret, size_t *deg_ret)
{
    /* Divisors of 2132: 1,2,4,13,26,41,52,82,164,533,1066,2132 */
    const uint64_t D[12] = {1,2,4,13,26,41,52,82,164,533,1066,2132};
    const size_t Ndiv = 12;

    /* Temporary structures */
    struct { seq_t *coeff; size_t deg; } polys[12];

    for (size_t i = 0; i < Ndiv; ++i)
    {
        uint64_t d = D[i];
        if (d == 1)                     // Φ1(x)=x−1
        {
            polys[i].coeff = alloc_array(2);
            polys[i].coeff[0] = -1;
            polys[i].coeff[1] = 1;
            polys[i].deg = 1;
        }
        else
        {
            /* numerator x^d − 1 */
            size_t deg = (size_t)d;
            seq_t *num = alloc_array(deg + 1);
            num[0] = -1;
            num[deg] = 1;
            size_t num_deg = deg;

            /* divide by Φ_k with k|d, k<d */
            for (size_t j = 0; j < i; ++j)
                if (d % D[j] == 0)
                    divide_in_place(&num, &num_deg,
                                    polys[j].coeff, polys[j].deg);

            polys[i].coeff = num;
            polys[i].deg   = num_deg;   /* should equal φ(d) */
        }
    }

    *phi_ret = polys[Ndiv-1].coeff;   /* Φ2132 */
    *deg_ret = polys[Ndiv-1].deg;

    /* Free intermediate arrays, except Φ2132 */
    for (size_t i = 0; i < Ndiv-1; ++i)
        free_array(polys[i].coeff);
}

/* ------------------------------------------------------------------ */
/* Periodic sequence array ------------------------------------------ */
static const idx_t PERIOD = 2132;
static seq_t *seq = NULL;

/* ------------------------------------------------------------------ */
/* Initialise the sequence once ------------------------------------- */
static void init_sequence(void)
{
    seq_t *phi;
    size_t phi_deg;
    build_phi_2132(&phi, &phi_deg);

    seq = alloc_array(PERIOD);
    seq[0] = 1;
    for (idx_t k = 1; k < PERIOD; ++k)
    {
        seq_t s = 0;
        size_t max_i = (k < phi_deg) ? (size_t)k : phi_deg;
        for (size_t i = 1; i <= max_i; ++i)
            s += phi[i] * seq[k - i];
        seq[k] = -s;
    }

    free_array(phi);   /* no longer needed */
}

/* ------------------------------------------------------------------ */
/* Public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq) init_sequence();       /* lazy initialisation */
    return seq[n % PERIOD];
}
