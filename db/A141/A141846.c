/*
 * A141846  Triangle read by rows: A051731 * A051953^(n-k) * 0^(n-k), 1 <= k
 *          <= n.
 * https://oeis.org/A141846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A141846
    The triangle is defined as
          T(r,k) = { k - φ(k)   if k divides r
                    { 0           otherwise
    where φ(k) is Euler's totient function.
    The sequence a(n) is obtained by reading the triangle
    row‑by‑row (offset 1).  Thus for a given index n we
    find the row r such that (r-1)r/2 < n ≤ r(r+1)/2,
    let k = n - (r-1)r/2 (the column position), and
    return T(r,k).  */

/*  Include standard headers (valid for both C11 and C++17)  */
#include <stdint.h>
#include <stdlib.h>

/*  idx_t and seq_t are defined by the build system.  */
static idx_t *phi = NULL;            /*  phi[i] = φ(i)  */
static idx_t  phi_size = 0;          /*  maximum index precomputed in phi[]  */

/*  Ensure that φ[0] … φ[limit] are computed.  */
static void
ensure_phi(idx_t limit)
{
    if (limit <= phi_size)
        return;

    idx_t new_size = limit + 1;
    idx_t *new_phi = (idx_t *)realloc(phi, new_size * sizeof(idx_t));
    if (!new_phi)
        return;                     /*  allocation failure – should never happen  */
    phi = new_phi;

    /*  initialise with p itself  */
    for (idx_t i = 0; i <= limit; ++i)
        phi[i] = i;

    /*  compute φ by the standard sieve  */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (phi[p] == p)           /*  p is prime  */
        {
            for (idx_t j = p; j <= limit; j += p)
                phi[j] -= phi[j] / p;
        }
    }
    phi[1] = 1;
    phi_size = limit;
}

/*  Return the nth term of A141846 (1‑based).  */
seq_t
a(idx_t n)
{
    /*  Find the row r such that (r-1)r/2 < n ≤ r(r+1)/2  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t prev_sum = (r - 1) * r / 2;    /*  number of terms before row r  */
    idx_t k = n - prev_sum;              /*  column index within the row  */

    /*  we need φ(k);  ensure k is pre‑computed  */
    ensure_phi(k);

    /*  if columns that do not divide the row index are zeros  */
    if (r % k != 0)
        return (seq_t)0;

    /*  value of the triangle entry  */
    seq_t val = (seq_t)(k - phi[k]);     /*  k - φ(k)  */
    return val;
}
