/*
 * A126890  Triangle read by rows: T(n,k) = n*(n+2*k+1)/2, 0 <= k <= n.
 * https://oeis.org/A126890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A126890 – Flattened triangle T(n,k)=n*(n+2k+1)/2, 0≤k≤n, read row‑wise. 
   The function computes a(n) by determining the row r and column k that
   correspond to the given index n. */
seq_t a(idx_t n)
{
    /* Find row r such that start(r) = r*(r+1)/2 <= n < start(r+1) */
    idx_t low = 0, high = 1;
    /* Exponential search to get an upper bound */
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
        high <<= 1;

    /* Binary search for the exact row */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                    /* row index */
    idx_t start_r = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - start_r;                /* column index */

    /* Compute T(r,k) = r*(r+2k+1)/2 with 128‑bit intermediate to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)r *
                            (((unsigned __int128)r) + ((unsigned __int128)2 * k) + 1);
    seq_t val = (seq_t)(prod / 2);
    return val;
}
